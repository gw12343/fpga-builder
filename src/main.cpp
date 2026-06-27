//
// Created by gabed on 4/14/2026.
//

#define SDL_MAIN_HANDLED
#include "Project/Project.h"

#include "UI/ConfigManager.h"
#include "UI/ErrorManager.h"
#include "UI/OutputViewer.h"
#include "UI/Renderer.h"
#include "UI/Toolbox.h"
#include "UI/Topbar.h"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

std::shared_ptr<Renderer> renderer;
std::shared_ptr<ErrorManager> error_manager;
std::shared_ptr<ConfigManager> config_manager;
std::shared_ptr<OutputViewer> output_viewer;
std::shared_ptr<Topbar> topbar;
std::shared_ptr<Toolbox> toolbox;

std::shared_ptr<Project> p;


void loop(void *arg) {
    renderer->StartFrame();


    toolbox->Render(p->GetSelectedModule(), config_manager);
    topbar->Render(p, error_manager, output_viewer);

    p->Render(error_manager, output_viewer);

    config_manager->Render(p->GetSelectedModule());
    output_viewer->Render();
    error_manager->Render(p->GetSelectedModule());

    renderer->EndFrame();
}


int main(int, char **) {
    renderer = std::make_shared<Renderer>();
    error_manager = std::make_shared<ErrorManager>();
    config_manager = std::make_shared<ConfigManager>();

    renderer->InitWindow(2000, 1600, "FPGA Builder");


    output_viewer = std::make_shared<OutputViewer>();
    topbar = std::make_shared<Topbar>();
    toolbox = std::make_shared<Toolbox>();

    p = std::make_shared<Project>(ASSET_BASE_PATH + "Project");

    if (p->GetSelectedModule().has_value()) {
        output_viewer->UpdateOutput(p->GetSelectedModule().value());
    }

#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop_arg(loop, nullptr, 0, 1);
#else
    // Main loop
    while (renderer->IsRunning()) {
        loop(nullptr);
    }
#endif

    // Clean up
    renderer->CloseWindow();
    return 0;
}
