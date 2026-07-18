//
// Created by gabed on 4/13/2026.
//

#pragma once
#include <SDL3/SDL_video.h>

#ifdef __EMSCRIPTEN__
#include <imgui.h>
#endif


class Renderer {
public:
    void InitWindow(int w, int h, const std::string &title);

    void CloseWindow() const;

    void StartFrame();
    void EndFrame() const;

    [[nodiscard]] bool IsRunning() const { return m_running; }

#ifdef __EMSCRIPTEN__
    /// Keep the canvas full-DOM and match CSS size / DPR / user zoom.
    /// Safe to call from emscripten resize callbacks and JS.
    void SyncWebCanvas(bool force = false);

    /// User zoom for the web build (1.0 = 100%). Ctrl/Cmd + scroll adjusts this.
    [[nodiscard]] float GetWebZoom() const { return m_web_zoom; }
    void SetWebZoom(float zoom);
    void ZoomByWheelDelta(float dom_delta_y);
#endif

private:
#ifdef __EMSCRIPTEN__
    void ApplyWebZoomToImGui();

    float m_web_zoom = 1.0f;
    int m_last_css_w = 0;
    int m_last_css_h = 0;
    float m_last_dpr = 0.0f;
    float m_last_zoom = 0.0f;
    int m_last_fb_w = 0;
    int m_last_fb_h = 0;
    bool m_have_base_style = false;
    ImGuiStyle m_base_style{};
#endif

    bool m_running = true;
    SDL_Window *m_window = nullptr;
    SDL_GLContext m_gl_context = nullptr;
};
