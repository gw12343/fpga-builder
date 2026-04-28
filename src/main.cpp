//
// Created by gabed on 4/14/2026.
//

#define SDL_MAIN_HANDLED

#include "CircuitSerializer.h"
#include "CopyPasteManager.h"
#include "Module.h"

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

    auto main_module = CircuitSerializer::LoadModule("../Project/circuit.json");

    const auto output_viewer = std::make_shared<OutputViewer>(main_module);
    const auto topbar = std::make_shared<Topbar>();
    const auto toolbox = std::make_shared<Toolbox>();


    // Main loop
    while (renderer->IsRunning()) {
        renderer->StartFrame();

        toolbox->Render(main_module, config_manager);
        topbar->Render(main_module, error_manager, output_viewer);
        main_module->Render(error_manager, cp_manager);
        config_manager->Render(main_module);
        output_viewer->Render();
        error_manager->Render(main_module);

        renderer->EndFrame();
    }

    // Clean up
    renderer->CloseWindow();
    return 0;
}
