//
// Created by gabed on 4/13/2026.
//

#define IMGUI_DISABLE_DEBUG_TOOLS
#include "Renderer.h"

#include <SDL3/SDL_init.h>
#include <SDL3/SDL_opengl.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_sdl3.h>
#include <algorithm>
#include <cmath>
#include <iostream>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <emscripten/html5.h>

namespace {
Renderer *g_web_renderer = nullptr;

EM_BOOL OnWebResize(int /*eventType*/, const EmscriptenUiEvent * /*event*/, void *userData) {
    if (auto *renderer = static_cast<Renderer *>(userData)) {
        renderer->SyncWebCanvas(true);
    }
    return EM_TRUE;
}

EM_BOOL OnWebFullscreen(int /*eventType*/, const EmscriptenFullscreenChangeEvent * /*event*/, void *userData) {
    if (auto *renderer = static_cast<Renderer *>(userData)) {
        renderer->SyncWebCanvas(true);
    }
    return EM_TRUE;
}
} // namespace

/// Called from shell.html on Ctrl/Cmd+wheel (DOM has reliable modifier flags).
/// Must not live in an anonymous namespace or it will not export as _fpga_web_on_ctrl_wheel.
/// Uses double so the JS number ABI is reliable without ccall helpers.
extern "C" EMSCRIPTEN_KEEPALIVE void fpga_web_on_ctrl_wheel(double delta_y) {
    if (g_web_renderer) {
        g_web_renderer->ZoomByWheelDelta(static_cast<float>(delta_y));
    }
}

void Renderer::SetWebZoom(float zoom) {
    m_web_zoom = std::clamp(zoom, 0.5f, 3.0f);
    SyncWebCanvas(true);
}

void Renderer::ZoomByWheelDelta(float dom_delta_y) {
    if (dom_delta_y == 0.0f) {
        return;
    }
    // DOM wheel: positive deltaY = scroll down = zoom out.
    constexpr float kStep = 1.1f;
    const float factor = dom_delta_y < 0.0f ? kStep : (1.0f / kStep);
    SetWebZoom(m_web_zoom * factor);
}

void Renderer::ApplyWebZoomToImGui() {
    if (ImGui::GetCurrentContext() == nullptr) {
        return;
    }

    if (m_have_base_style) {
        // Re-derive from the post-init style so ScaleAllSizes stays idempotent.
        ImGui::GetStyle() = m_base_style;
        if (m_web_zoom != 1.0f) {
            ImGui::GetStyle().ScaleAllSizes(m_web_zoom);
        }
    }
    ImGui::GetIO().FontGlobalScale = m_web_zoom;
}

void Renderer::SyncWebCanvas(bool force) {
    if (!m_window) {
        return;
    }

    // 1) Force CSS full-DOM. SDL and emscripten often write fixed px sizes on
    //    the canvas which break under browser zoom (gray gutters, cornered view).
    // 2) Measure the *laid-out* canvas box (handles browser +/- zoom correctly).
    // 3) Set only the backing-store attributes; leave CSS as 100%.
    const double css_w = EM_ASM_DOUBLE({
        const c = document.getElementById('canvas');
        if (!c) {
            return window.innerWidth || 1;
        }
        // Strip fixed px that SDL/emscripten may have applied.
        c.style.setProperty('width', '100%', 'important');
        c.style.setProperty('height', '100%', 'important');
        c.style.setProperty('top', '0', 'important');
        c.style.setProperty('left', '0', 'important');
        c.style.setProperty('right', '0', 'important');
        c.style.setProperty('bottom', '0', 'important');
        c.style.setProperty('position', 'fixed', 'important');
        c.style.removeProperty('max-width');
        c.style.removeProperty('max-height');
        // Force a layout pass, then measure the real CSS box.
        const rect = c.getBoundingClientRect();
        let w = rect.width;
        if (!(w > 0)) {
            w = c.clientWidth || window.innerWidth || document.documentElement.clientWidth || 1;
        }
        return w;
    });
    const double css_h = EM_ASM_DOUBLE({
        const c = document.getElementById('canvas');
        if (!c) {
            return window.innerHeight || 1;
        }
        const rect = c.getBoundingClientRect();
        let h = rect.height;
        if (!(h > 0)) {
            h = c.clientHeight || window.innerHeight || document.documentElement.clientHeight || 1;
        }
        return h;
    });

    const float dpr = static_cast<float>(EM_ASM_DOUBLE({ return window.devicePixelRatio || 1; }));
    const int css_w_i = std::max(1, static_cast<int>(std::lround(css_w)));
    const int css_h_i = std::max(1, static_cast<int>(std::lround(css_h)));

    // Backing store tracks device pixels * app zoom. CSS box stays full-window.
    const float pixel_scale = std::max(0.5f, dpr * m_web_zoom);
    const int fb_w = std::max(1, static_cast<int>(std::lround(static_cast<double>(css_w_i) * pixel_scale)));
    const int fb_h = std::max(1, static_cast<int>(std::lround(static_cast<double>(css_h_i) * pixel_scale)));

    const bool size_changed = css_w_i != m_last_css_w || css_h_i != m_last_css_h || dpr != m_last_dpr ||
                              fb_w != m_last_fb_w || fb_h != m_last_fb_h;
    const bool zoom_changed = m_web_zoom != m_last_zoom;

    if (!force && !size_changed && !zoom_changed) {
        return;
    }

    m_last_css_w = css_w_i;
    m_last_css_h = css_h_i;
    m_last_dpr = dpr;
    m_last_zoom = m_web_zoom;
    m_last_fb_w = fb_w;
    m_last_fb_h = fb_h;

    // Logical window for SDL/ImGui mouse + layout (CSS pixels).
    SDL_SetWindowSize(m_window, css_w_i, css_h_i);

    // SDL often resets the canvas buffer and/or inline CSS on SetWindowSize —
    // re-assert both every time.
    emscripten_set_canvas_element_size("#canvas", fb_w, fb_h);
    EM_ASM({
        const c = document.getElementById('canvas');
        if (!c) return;
        c.style.setProperty('width', '100%', 'important');
        c.style.setProperty('height', '100%', 'important');
        c.style.setProperty('position', 'fixed', 'important');
        c.style.setProperty('top', '0', 'important');
        c.style.setProperty('left', '0', 'important');
        // Ensure attribute size matches (some SDL builds only touch attributes).
        if (c.width !== $0) c.width = $0;
        if (c.height !== $1) c.height = $1;
    }, fb_w, fb_h);

    if (zoom_changed || force) {
        ApplyWebZoomToImGui();
    }
}
#endif

void Renderer::InitWindow(const int w, const int h, const std::string &title) {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return;
    }


#ifdef __EMSCRIPTEN__
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
#endif


    m_window = SDL_CreateWindow(title.c_str(), w, h,
                                SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIGH_PIXEL_DENSITY);


    SDL_Surface *iconSurface = SDL_LoadBMP("resources/icon.bmp");

    if (iconSurface != nullptr) {
        SDL_SetWindowIcon(m_window, iconSurface);
        SDL_DestroySurface(iconSurface);
    } else {
        std::cerr << "Failed to load icon: %s" << SDL_GetError() << "\n";
    }

    m_gl_context = SDL_GL_CreateContext(m_window);
    SDL_GL_MakeCurrent(m_window, m_gl_context);
    SDL_GL_SetSwapInterval(1);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    io.IniFilename = (new std::string(ASSET_BASE_PATH + "imgui.ini"))->c_str();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    ImGui::StyleColorsDark();


    constexpr float UI_SCALE = 2;
    ImGuiStyle &style = ImGui::GetStyle();

    style.ScaleAllSizes(UI_SCALE);


    // ImFontConfig roboto_config;
    // roboto_config.MergeMode = true;
    // roboto_config.PixelSnapH = true;

    const auto roboto = ASSET_BASE_PATH + "resources/Roboto-Regular.ttf";
    io.Fonts->AddFontFromFileTTF(roboto.c_str(), 12.0f * UI_SCALE);

    ImFontConfig fa_config;
    fa_config.MergeMode = true;
    fa_config.PixelSnapH = true;
    fa_config.GlyphMinAdvanceX = 12.0f;

    static constexpr ImWchar ICON_RANGES[] = {ICON_MIN_FA, ICON_MAX_FA, 0};
    const auto fa_solid = ASSET_BASE_PATH + "resources/fa-solid-900.ttf";

    io.Fonts->AddFontFromFileTTF(fa_solid.c_str(), 12.0f * UI_SCALE, &fa_config, ICON_RANGES);


    io.FontGlobalScale = 1.0f;


    ImGui_ImplSDL3_InitForOpenGL(m_window, m_gl_context);

#ifdef __EMSCRIPTEN__
    const char *glsl_version = "#version 300 es";
#else
    const char *glsl_version = "#version 130";
#endif

    ImGui_ImplOpenGL3_Init(glsl_version);

    style.ChildRounding = 0;
    style.GrabRounding = 0;
    style.FrameRounding = 2;
    style.PopupRounding = 0;
    style.ScrollbarRounding = 0;
    style.TabRounding = 2;
    style.WindowRounding = 0;
    style.FramePadding = {4, 4};

    style.WindowTitleAlign = {0.0, 0.5};
    style.ColorButtonPosition = ImGuiDir_Left;

    ImVec4 *colors = ImGui::GetStyle().Colors;

    colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
    colors[ImGuiCol_TextDisabled] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
    colors[ImGuiCol_WindowBg] = ImVec4(0.09f, 0.09f, 0.09f, 0.94f);
    colors[ImGuiCol_ChildBg] = ImVec4(0.11f, 0.11f, 0.11f, 1.00f);
    colors[ImGuiCol_PopupBg] = ImVec4(0.11f, 0.11f, 0.11f, 0.94f);
    colors[ImGuiCol_Border] = ImVec4(0.07f, 0.08f, 0.08f, 1.00f);
    colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_FrameBg] = ImVec4(0.35f, 0.35f, 0.35f, 0.54f);
    colors[ImGuiCol_FrameBgHovered] = ImVec4(0.31f, 0.29f, 0.27f, 1.00f);
    colors[ImGuiCol_FrameBgActive] = ImVec4(0.40f, 0.36f, 0.33f, 0.67f);
    colors[ImGuiCol_TitleBg] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
    colors[ImGuiCol_TitleBgActive] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
    colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.61f);
    colors[ImGuiCol_MenuBarBg] = ImVec4(0.18f, 0.18f, 0.18f, 0.94f);
    colors[ImGuiCol_ScrollbarBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.16f);
    colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.24f, 0.22f, 0.21f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.31f, 0.29f, 0.27f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.40f, 0.36f, 0.33f, 1.00f);
    colors[ImGuiCol_CheckMark] = ImVec4(0.84f, 0.84f, 0.84f, 1.00f);
    colors[ImGuiCol_SliderGrab] = ImVec4(0.43f, 0.43f, 0.43f, 1.00f);
    colors[ImGuiCol_SliderGrabActive] = ImVec4(0.55f, 0.55f, 0.55f, 1.00f);

    colors[ImGuiCol_Button] = ImVec4(0.3, 0.3, 0.3, 1.0);
    colors[ImGuiCol_ButtonHovered] = ImVec4(0.35, 0.35, 0.35, 1.0);
    colors[ImGuiCol_ButtonActive] = ImVec4(0.4, 0.4, 0.4, 1.0);

    colors[ImGuiCol_Header] = ImVec4(0.84f, 0.36f, 0.05f, 0.00f);
    colors[ImGuiCol_HeaderHovered] = ImVec4(0.25f, 0.25f, 0.25f, 0.80f);
    colors[ImGuiCol_HeaderActive] = ImVec4(0.42f, 0.42f, 0.42f, 1.00f);
    colors[ImGuiCol_Separator] = ImVec4(0.35f, 0.35f, 0.35f, 0.50f);
    colors[ImGuiCol_SeparatorHovered] = ImVec4(0.31f, 0.29f, 0.27f, 0.78f);
    colors[ImGuiCol_SeparatorActive] = ImVec4(0.40f, 0.36f, 0.33f, 1.00f);
    colors[ImGuiCol_ResizeGrip] = ImVec4(1.00f, 1.00f, 1.00f, 0.25f);
    colors[ImGuiCol_ResizeGripHovered] = ImVec4(1.00f, 1.00f, 1.00f, 0.40f);
    colors[ImGuiCol_ResizeGripActive] = ImVec4(1.00f, 1.00f, 1.00f, 0.95f);
    colors[ImGuiCol_InputTextCursor] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
    colors[ImGuiCol_TabHovered] = ImVec4(0.56f, 0.15f, 0.15f, 1.00f);
    colors[ImGuiCol_Tab] = ImVec4(0.18f, 0.18f, 0.18f, 1.00f);
    colors[ImGuiCol_TabSelected] = ImVec4(0.48f, 0.14f, 0.14f, 1.00f);
    colors[ImGuiCol_TabSelectedOverline] = ImVec4(0.48f, 0.14f, 0.14f, 1.00f);
    colors[ImGuiCol_TabDimmed] = ImVec4(0.07f, 0.10f, 0.15f, 0.97f);
    colors[ImGuiCol_TabDimmedSelected] = ImVec4(0.42f, 0.14f, 0.14f, 1.00f);
    colors[ImGuiCol_TabDimmedSelectedOverline] = ImVec4(0.50f, 0.50f, 0.50f, 0.00f);
    colors[ImGuiCol_DockingPreview] = ImVec4(0.98f, 0.26f, 0.26f, 0.70f);
    colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
    colors[ImGuiCol_PlotLines] = ImVec4(0.66f, 0.60f, 0.52f, 1.00f);
    colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.98f, 0.29f, 0.20f, 1.00f);
    colors[ImGuiCol_PlotHistogram] = ImVec4(0.60f, 0.59f, 0.10f, 1.00f);
    colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.72f, 0.73f, 0.15f, 1.00f);
    colors[ImGuiCol_TableHeaderBg] = ImVec4(0.19f, 0.19f, 0.20f, 1.00f);
    colors[ImGuiCol_TableBorderStrong] = ImVec4(0.31f, 0.31f, 0.35f, 1.00f);
    colors[ImGuiCol_TableBorderLight] = ImVec4(0.23f, 0.23f, 0.25f, 1.00f);
    colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
    colors[ImGuiCol_TextLink] = ImVec4(0.98f, 0.26f, 0.26f, 1.00f);
    colors[ImGuiCol_TextSelectedBg] = ImVec4(0.27f, 0.52f, 0.53f, 0.35f);
    colors[ImGuiCol_TreeLines] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
    colors[ImGuiCol_DragDropTarget] = ImVec4(0.60f, 0.59f, 0.10f, 0.90f);
    colors[ImGuiCol_DragDropTargetBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_UnsavedMarker] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
    colors[ImGuiCol_NavCursor] = ImVec4(0.51f, 0.65f, 0.60f, 1.00f);
    colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
    colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
    colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.11f, 0.13f, 0.13f, 0.35f);

#ifdef __EMSCRIPTEN__
    // Snapshot style at zoom=1.0 so ctrl+scroll can re-scale without drift.
    m_base_style = ImGui::GetStyle();
    m_have_base_style = true;
    g_web_renderer = this;

    // Initial full-DOM size (overrides the desktop 2000x1600 request).
    SyncWebCanvas(true);
    emscripten_set_resize_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, this, false, OnWebResize);
    emscripten_set_fullscreenchange_callback(EMSCRIPTEN_EVENT_TARGET_DOCUMENT, this, false, OnWebFullscreen);

    // Browser zoom and mobile chrome often update visualViewport without a
    // reliable window.resize; poll path in StartFrame also covers this.
    EM_ASM({
        const fire = () => { window.dispatchEvent(new Event('resize')); };
        window.addEventListener('resize', fire);
        if (window.visualViewport) {
            window.visualViewport.addEventListener('resize', fire);
            window.visualViewport.addEventListener('scroll', fire);
        }
        // devicePixelRatio changes with browser zoom in Chromium.
        if (window.matchMedia) {
            let mql = window.matchMedia('screen and (resolution: ' + window.devicePixelRatio + 'dppx)');
            const onDpr = () => {
                try {
                    mql.removeEventListener('change', onDpr);
                } catch (err) {
                    try { mql.removeListener(onDpr); } catch (e2) {}
                }
                mql = window.matchMedia('screen and (resolution: ' + window.devicePixelRatio + 'dppx)');
                try {
                    mql.addEventListener('change', onDpr);
                } catch (err) {
                    try { mql.addListener(onDpr); } catch (e2) {}
                }
                fire();
            };
            try {
                mql.addEventListener('change', onDpr);
            } catch (err) {
                try { mql.addListener(onDpr); } catch (e2) {}
            }
        }
    });
#endif
}

void Renderer::CloseWindow() const {
#ifdef __EMSCRIPTEN__
    if (g_web_renderer == this) {
        g_web_renderer = nullptr;
    }
#endif

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();

    SDL_GL_DestroyContext(m_gl_context);
    SDL_DestroyWindow(m_window);
    SDL_Quit();
}

SDL_Event event;
void Renderer::StartFrame() {
#ifdef __EMSCRIPTEN__
    // Pick up DPR / viewport changes that don't always fire resize events
    // (e.g. moving across monitors, some browser UI chrome toggles).
    SyncWebCanvas(false);
#endif

    while (SDL_PollEvent(&event)) {
        ImGui_ImplSDL3_ProcessEvent(&event);

        if (event.type == SDL_EVENT_QUIT)
            m_running = false;
#ifdef __EMSCRIPTEN__
        if (event.type == SDL_EVENT_WINDOW_RESIZED || event.type == SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED) {
            SyncWebCanvas(true);
        }
#endif
    }

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
#ifdef __EMSCRIPTEN__
    // Own DisplaySize / FramebufferScale: CSS logical size vs canvas buffer.
    // (SDL's pixel-size query often ignores our DPR/zoom buffer sizing.)
    {
        ImGuiIO &io = ImGui::GetIO();
        if (m_last_css_w > 0 && m_last_css_h > 0 && m_last_fb_w > 0 && m_last_fb_h > 0) {
            io.DisplaySize = ImVec2(static_cast<float>(m_last_css_w), static_cast<float>(m_last_css_h));
            io.DisplayFramebufferScale =
                    ImVec2(static_cast<float>(m_last_fb_w) / static_cast<float>(m_last_css_w),
                           static_cast<float>(m_last_fb_h) / static_cast<float>(m_last_css_h));
        }
    }
#endif
    ImGui::NewFrame();


    ImGuiWindowFlags flags = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoBringToFrontOnFocus;

    const auto *viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);
    ImGui::SetNextWindowViewport(viewport->ID);

    flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
             ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;


    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

    ImGui::Begin("DockspaceWindow", nullptr, flags);
    ImGui::PopStyleVar(2);


    const ImGuiID dockspace_id = ImGui::GetID("Dockspace");
    ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode);
    ImGui::End();
}

void Renderer::EndFrame() const {
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 5.f);
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(43.f / 255.f, 43.f / 255.f, 43.f / 255.f, 100.f / 255.f));
    ImGui::RenderNotifications();

    ImGui::PopStyleVar(1);
    ImGui::PopStyleColor(1);

    ImGui::Render();

    int fb_width = 0, fb_height = 0;
#ifdef __EMSCRIPTEN__
    // Prefer the real canvas buffer size we set in SyncWebCanvas so the
    // viewport stays correct if SDL's pixel-size query lags a frame.
    emscripten_get_canvas_element_size("#canvas", &fb_width, &fb_height);
    if (fb_width <= 0 || fb_height <= 0) {
        SDL_GetWindowSizeInPixels(m_window, &fb_width, &fb_height);
    }
#else
    SDL_GetWindowSizeInPixels(m_window, &fb_width, &fb_height);
#endif

    glViewport(0, 0, fb_width, fb_height);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
#ifndef __EMSCRIPTEN__
    ImGuiIO &io_ref = ImGui::GetIO();
    if (io_ref.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
        SDL_Window *backup_current_window = SDL_GL_GetCurrentWindow();
        SDL_GLContext backup_current_context = SDL_GL_GetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        SDL_GL_MakeCurrent(backup_current_window, backup_current_context);
    }
#endif

    SDL_GL_SwapWindow(m_window);
}
