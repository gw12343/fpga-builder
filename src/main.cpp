//
// Created by gabed on 4/14/2026.
//

#define SDL_MAIN_HANDLED
#include "CopyPasteManager.h"
#include "Project/Project.h"

#include "UI/ConfigManager.h"
#include "UI/ErrorManager.h"
#include "UI/OutputViewer.h"
#include "UI/Renderer.h"
#include "UI/Toolbox.h"
#include "UI/Topbar.h"


int main(int, char **) {
    const auto renderer = std::make_shared<Renderer>();
    const auto error_manager = std::make_shared<ErrorManager>();
    const auto config_manager = std::make_shared<ConfigManager>();
    const auto cp_manager = std::make_shared<CopyPasteManager>();

    renderer->InitWindow(2000, 1600, "FPGA Builder");


    const auto output_viewer = std::make_shared<OutputViewer>();
    const auto topbar = std::make_shared<Topbar>();
    const auto toolbox = std::make_shared<Toolbox>();

    const auto p = std::make_shared<Project>("../Project");


    // Main loop
    while (renderer->IsRunning()) {
        renderer->StartFrame();


        toolbox->Render(p->GetSelectedModule(), config_manager);
        topbar->Render(p, error_manager, output_viewer);

        p->Render(error_manager, cp_manager);

        config_manager->Render(p->GetSelectedModule());
        output_viewer->Render();
        error_manager->Render(p->GetSelectedModule());

        renderer->EndFrame();
    }

    // Clean up
    renderer->CloseWindow();
    return 0;
}
