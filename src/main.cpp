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
#include "misc/cpp/imgui_stdlib.h"

#define NODE_CONFIG_TITLE "Configure Node"
bool popup = false;
std::shared_ptr<Node> curr;
int RequestNumBits(const std::shared_ptr<Module> &module) {
    if (popup) {
        ImGui::OpenPopup(NODE_CONFIG_TITLE);
    }

    if (ImGui::BeginPopupModal(NODE_CONFIG_TITLE, &popup, ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::Text("Configuring new %s node.", curr->name.c_str());
        ImGui::Separator();

        curr->RenderConfiguration();

        if (ImGui::Button("Done")) {
            popup = false;

            curr->InitPinsAfterConfig();
            module->nodes.push_back(curr);
            curr.reset();
        }

        ImGui::EndPopup();
    }


    return 0;
}


int main(int, char **) {
    const auto renderer = std::make_shared<Renderer>();
    const auto error_manager = std::make_shared<ErrorManager>();

    renderer->InitWindow(800, 600, "FPGA Builder");

    auto main_module = CircuitSerializer::LoadModule("../Project/circuit.json");


    // Main loop
    while (renderer->IsRunning()) {
        renderer->StartFrame();

        main_module->Render(error_manager);


        ImGui::Begin("Build");
        RequestNumBits(main_module);

        ImGui::InputText("Module Name", &main_module->name);

        ImGui::BeginChild("options", ImVec2(0, 150), ImGuiChildFlags_AutoResizeX);
        {
            std::shared_ptr<Node> new_node;

            if (ImGui::Button("OR", ImVec2(150, 150))) {
                new_node = std::make_shared<OrNode>(main_module.get(), GUID::generate_guid());
            }

            ImGui::SameLine();

            if (ImGui::Button("NOR", ImVec2(150, 150))) {
                new_node = std::make_shared<NorNode>(main_module.get(), GUID::generate_guid());
            }

            ImGui::SameLine();
            if (ImGui::Button("AND", ImVec2(150, 150))) {
                new_node = std::make_shared<AndNode>(main_module.get(), GUID::generate_guid());
            }

            ImGui::SameLine();
            if (ImGui::Button("XOrNode", ImVec2(150, 150))) {
                new_node = std::make_shared<XOrNode>(main_module.get(), GUID::generate_guid());
            }

            ImGui::SameLine();
            if (ImGui::Button("MP Node", ImVec2(150, 150))) {
                new_node = std::make_shared<MultiplexerNode>(main_module.get(), GUID::generate_guid(), 4);
            }
            ImGui::SameLine();
            if (ImGui::Button("Adder Node", ImVec2(150, 150))) {
                new_node = std::make_shared<AdderNode>(main_module.get(), GUID::generate_guid(), 4);
            }
            ImGui::SameLine();

            if (ImGui::Button("LT Node", ImVec2(150, 150))) {
                new_node = std::make_shared<LiteralNode>(main_module.get(), GUID::generate_guid(), 4, 0);
            }

            ImGui::SameLine();

            if (ImGui::Button("Splitter Node", ImVec2(150, 150))) {
                new_node = std::make_shared<SplitterNode>(main_module.get());
            }
            ImGui::SameLine();

            if (ImGui::Button("Combiner Node", ImVec2(150, 150))) {
                new_node = std::make_shared<CombinerNode>(main_module.get());
            }
            ImGui::SameLine();

            if (ImGui::Button("Counter Node", ImVec2(150, 150))) {
                new_node = std::make_shared<CounterNode>(main_module.get(), GUID::generate_guid(), 4);
            }
            ImGui::SameLine();

            if (ImGui::Button("Register Node", ImVec2(150, 150))) {
                new_node = std::make_shared<RegisterNode>(main_module.get(), GUID::generate_guid(), 4);
            }
            ImGui::SameLine();
            if (ImGui::Button("DFF Node", ImVec2(150, 150))) {
                new_node = std::make_shared<DFFNode>(main_module.get(), GUID::generate_guid());
            }
            ImGui::SameLine();
            if (ImGui::Button("Debounce Node", ImVec2(150, 150))) {
                new_node = std::make_shared<DebounceNode>(main_module.get(), GUID::generate_guid());
            }
            ImGui::SameLine();
            if (ImGui::Button("Edge Node", ImVec2(150, 150))) {
                new_node = std::make_shared<EdgeNode>(main_module.get(), GUID::generate_guid());
            }
            ImGui::SameLine();

            if (ImGui::Button("Clock Node " ICON_FA_WAVE_SQUARE, ImVec2(150, 150))) {
                new_node = std::make_shared<ClockNode>(main_module.get(), GUID::generate_guid());
            }
            ImGui::SameLine();
            if (ImGui::Button("Not Node", ImVec2(150, 150))) {
                new_node = std::make_shared<NotNode>(main_module.get(), GUID::generate_guid());
            }

            if (new_node) {
                if (new_node->HasConfiguration()) {
                    popup = true;
                    curr = new_node;
                } else {
                    main_module->nodes.push_back(new_node);
                }
            }
        }
        ImGui::EndChild();

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
                CircuitSerializer::SaveModule(main_module, "../Project/circuit.json");
            }

            ImGui::SameLine();

            if (ImGui::Button("Load Circuit")) {
                main_module = CircuitSerializer::LoadModule("../Project/circuit.json");
            }
        }

        ImGui::End();

        error_manager->Render(main_module);
        renderer->EndFrame();
    }

    // Clean up
    renderer->CloseWindow();
    return 0;
}
