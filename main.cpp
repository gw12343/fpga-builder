#define SDL_MAIN_HANDLED


#include "Module.h"
#include "Renderer.h"
#include "Codegen/Codegen.h"
#include "Codegen/TraversePrint.h"
#include "Default/OutputNode.h"
#include <nlohmann/json.hpp>

#include "CircuitSerializer.h"
#include "ErrorManager.h"
#include "GUID.h"
#include "Default/DebounceNode.h"
#include "Default/DFFNode.h"
#include "Default/EdgeNode.h"
#include "Default/LiteralNode.h"
#include "Default/MultiplexerNode.h"
#include "Default/BinaryOperator/AndNode.h"
#include "Default/BinaryOperator/OrNode.h"
#include "Default/BinaryOperator/XOrNode.h"
#include "Default/BinaryOperator/NorNode.h"


int main(int, char**) {
    const auto renderer = std::make_shared<Renderer>();
    const auto error_manager = std::make_shared<ErrorManager>();

    renderer->InitWindow(800, 600, "FPGA Builder");

    auto main_module = CircuitSerializer::LoadModule("../circuit.json");



    // Main loop
    while (renderer->IsRunning())
    {
        renderer->StartFrame();

        main_module->Update();
        main_module->Render(error_manager);

        ImGui::Begin("Build");

        if (ImGui::Button("OR")) {
             main_module->nodes.push_back(std::make_unique<OrNode>(main_module.get(), GUID::generate_guid()));

        }

        if (ImGui::Button("NOR")) {
             main_module->nodes.push_back(std::make_unique<NorNode>(main_module.get(), GUID::generate_guid()));

        }

        if (ImGui::Button("AND")) {
            main_module->nodes.push_back(std::make_unique<AndNode>(main_module.get(), GUID::generate_guid()));

        }

        if (ImGui::Button("XOrNode")) {
            main_module->nodes.push_back(std::make_unique<XOrNode>(main_module.get(), GUID::generate_guid()));

        }

        if (ImGui::Button("MP Node")) {
            main_module->nodes.push_back(std::make_unique<MultiplexerNode>(main_module.get(), GUID::generate_guid()));

        }

        if (ImGui::Button("LT Node")) {
            main_module->nodes.push_back(std::make_unique<LiteralNode>(main_module.get(), GUID::generate_guid(), 0));
        }
        if (ImGui::Button("DFF Node")) {
            main_module->nodes.push_back(std::make_unique<DFFNode>(main_module.get(), GUID::generate_guid()));

        }
        if (ImGui::Button("Debounce Node")) {
            main_module->nodes.push_back(std::make_unique<DebounceNode>(main_module.get(), GUID::generate_guid()));
        }
        if (ImGui::Button("Edge Node")) {
            main_module->nodes.push_back(std::make_unique<EdgeNode>(main_module.get(), GUID::generate_guid()));
        }


        if (ImGui::Button("Print Circuit")) {
            TraversePrint v;
            main_module->nodes[0]->accept(v);

        }

        ImGui::SameLine();

        if (ImGui::Button("Export Circuit")) {
            Codegen c(error_manager);

            c.GenerateCode(main_module);
        }

        ImGui::SameLine();

        if (ImGui::Button("Save Circuit")) {
            CircuitSerializer::SaveModule(main_module, "../circuit.json");
        }

        ImGui::SameLine();

        if (ImGui::Button("Load Circuit")) {
            main_module = CircuitSerializer::LoadModule("../circuit.json");

        }

        ImGui::End();
        error_manager->Render(main_module);
        renderer->EndFrame();
    }

    // Clean up
    renderer->CloseWindow();
    return 0;
}
