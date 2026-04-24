#define SDL_MAIN_HANDLED


#include <nlohmann/json.hpp>
#include "Codegen/Codegen.h"
#include "Codegen/TraversePrint.h"
#include "Default/OutputNode.h"
#include "Module.h"
#include "Renderer.h"

#include "CircuitSerializer.h"
#include "Default/AdderNode.h"
#include "Default/BinaryOperator/AndNode.h"
#include "Default/BinaryOperator/NorNode.h"
#include "Default/BinaryOperator/OrNode.h"
#include "Default/BinaryOperator/XOrNode.h"
#include "Default/ClockNode.h"
#include "Default/CombinerNode.h"
#include "Default/CounterNode.h"
#include "Default/DFFNode.h"
#include "Default/DebounceNode.h"
#include "Default/EdgeNode.h"
#include "Default/LiteralNode.h"
#include "Default/MultiplexerNode.h"
#include "Default/RegisterNode.h"
#include "Default/SplitterNode.h"
#include "Default/UnaryOperator/NotNode.h"
#include "ErrorManager.h"
#include "GUID.h"
#include "IconsFontAwesome6.h"

int main(int, char **) {
    const auto renderer = std::make_shared<Renderer>();
    const auto error_manager = std::make_shared<ErrorManager>();

    renderer->InitWindow(800, 600, "FPGA Builder");

    auto main_module = CircuitSerializer::LoadModule("../circuit.json");


    // Main loop
    while (renderer->IsRunning()) {
        renderer->StartFrame();

        main_module->Render(error_manager);

        ImGui::Begin("Build");

        ImGui::BeginChild("options", ImVec2(0, 150), ImGuiChildFlags_AutoResizeX);
        {
            if (ImGui::Button("OR", ImVec2(150, 150))) {
                main_module->nodes.push_back(std::make_unique<OrNode>(main_module.get(), GUID::generate_guid()));
            }

            ImGui::SameLine();

            if (ImGui::Button("NOR", ImVec2(150, 150))) {
                main_module->nodes.push_back(std::make_unique<NorNode>(main_module.get(), GUID::generate_guid()));
            }

            ImGui::SameLine();
            if (ImGui::Button("AND", ImVec2(150, 150))) {
                main_module->nodes.push_back(std::make_unique<AndNode>(main_module.get(), GUID::generate_guid()));
            }

            ImGui::SameLine();
            if (ImGui::Button("XOrNode", ImVec2(150, 150))) {
                main_module->nodes.push_back(std::make_unique<XOrNode>(main_module.get(), GUID::generate_guid()));
            }

            ImGui::SameLine();
            if (ImGui::Button("MP Node", ImVec2(150, 150))) {
                main_module->nodes.push_back(
                        std::make_unique<MultiplexerNode>(main_module.get(), GUID::generate_guid(), 4));
            }
            ImGui::SameLine();
            if (ImGui::Button("Adder Node", ImVec2(150, 150))) {
                main_module->nodes.push_back(std::make_unique<AdderNode>(main_module.get(), GUID::generate_guid(), 4));
            }
            ImGui::SameLine();

            if (ImGui::Button("LT Node", ImVec2(150, 150))) {
                main_module->nodes.push_back(
                        std::make_unique<LiteralNode>(main_module.get(), GUID::generate_guid(), 0));
            }

            ImGui::SameLine();

            if (ImGui::Button("Splitter Node", ImVec2(150, 150))) {
                main_module->nodes.push_back(
                        std::make_unique<SplitterNode>(main_module.get(), GUID::generate_guid(), 4));
            }
            ImGui::SameLine();

            if (ImGui::Button("Combiner Node", ImVec2(150, 150))) {
                main_module->nodes.push_back(
                        std::make_unique<CombinerNode>(main_module.get(), GUID::generate_guid(), 4));
            }
            ImGui::SameLine();

            if (ImGui::Button("Counter Node", ImVec2(150, 150))) {
                main_module->nodes.push_back(
                        std::make_unique<CounterNode>(main_module.get(), GUID::generate_guid(), 4));
            }
            ImGui::SameLine();

            if (ImGui::Button("Register Node", ImVec2(150, 150))) {
                main_module->nodes.push_back(
                        std::make_unique<RegisterNode>(main_module.get(), GUID::generate_guid(), 4));
            }
            ImGui::SameLine();
            if (ImGui::Button("DFF Node", ImVec2(150, 150))) {
                main_module->nodes.push_back(std::make_unique<DFFNode>(main_module.get(), GUID::generate_guid()));
            }
            ImGui::SameLine();
            if (ImGui::Button("Debounce Node", ImVec2(150, 150))) {
                main_module->nodes.push_back(std::make_unique<DebounceNode>(main_module.get(), GUID::generate_guid()));
            }
            ImGui::SameLine();
            if (ImGui::Button("Edge Node", ImVec2(150, 150))) {
                main_module->nodes.push_back(std::make_unique<EdgeNode>(main_module.get(), GUID::generate_guid()));
            }
            ImGui::SameLine();

            if (ImGui::Button("Clock Node " ICON_FA_WAVE_SQUARE, ImVec2(150, 150))) {
                main_module->nodes.push_back(std::make_unique<ClockNode>(main_module.get(), GUID::generate_guid()));
            }
            ImGui::SameLine();
            if (ImGui::Button("Not Node", ImVec2(150, 150))) {
                main_module->nodes.push_back(std::make_unique<NotNode>(main_module.get(), GUID::generate_guid()));
            }
        }
        ImGui::EndChild();

        // ImGui::BeginChild("menu");
        {
            if (ImGui::Button("Print Circuit")) {
                TraversePrint v;
                main_module->nodes[0]->accept(v, 0);
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
        }
        // ImGui::EndChild();

        ImGui::End();

        error_manager->Render(main_module);
        renderer->EndFrame();
    }

    // Clean up
    renderer->CloseWindow();
    return 0;
}
