//
// Created by gabed on 4/14/2026.
//

#define SDL_MAIN_HANDLED

#include "CircuitSerializer.h"
#include "Module.h"

#include "UI/ConfigManager.h"
#include "UI/ErrorManager.h"
#include "UI/Renderer.h"
#include "UI/Toolbox.h"
#include "UI/Topbar.h"


int main(int, char **) {
    const auto renderer = std::make_shared<Renderer>();
    const auto error_manager = std::make_shared<ErrorManager>();
    const auto config_manager = std::make_shared<ConfigManager>();
    const auto toolbox = std::make_shared<Toolbox>();
    const auto topbar = std::make_shared<Topbar>();

    renderer->InitWindow(2000, 1600, "FPGA Builder");

    auto main_module = CircuitSerializer::LoadModule("../Project/circuit.json");


    // Main loop
    while (renderer->IsRunning()) {
        renderer->StartFrame();

        toolbox->Render(main_module, config_manager);
        topbar->Render(main_module, error_manager);
        main_module->Render(error_manager);
        config_manager->Render(main_module);
        error_manager->Render(main_module);

        renderer->EndFrame();
    }

    // Clean up
    renderer->CloseWindow();
    return 0;
}
