//
// Created by Gabe on 4/26/2026.
//

#include "Toolbox.h"

#include "CircuitSerializer.h"
#include "Codegen/Codegen.h"
#include "Codegen/TraversePrint.h"
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
#include "IconsFontAwesome6.h"
#include "misc/cpp/imgui_stdlib.h"


void Toolbox::Render(std::shared_ptr<Module> &module, const std::shared_ptr<ConfigManager> &config_manager,
                     const std::shared_ptr<ErrorManager> &error_manager) {
    ImGui::Begin("Toolbox");

    ImGui::InputText("Module Name", &module->name);

    ImGui::BeginChild("options", ImVec2(0, 150), ImGuiChildFlags_AutoResizeX);
    {
        std::shared_ptr<Node> new_node;

        if (ImGui::Button("OR", ImVec2(150, 150))) {
            new_node = std::make_shared<OrNode>(module.get());
        }

        ImGui::SameLine();

        if (ImGui::Button("NOR", ImVec2(150, 150))) {
            new_node = std::make_shared<NorNode>(module.get());
        }

        ImGui::SameLine();
        if (ImGui::Button("AND", ImVec2(150, 150))) {
            new_node = std::make_shared<AndNode>(module.get());
        }

        ImGui::SameLine();
        if (ImGui::Button("XOrNode", ImVec2(150, 150))) {
            new_node = std::make_shared<XOrNode>(module.get());
        }

        ImGui::SameLine();
        if (ImGui::Button("MP Node", ImVec2(150, 150))) {
            new_node = std::make_shared<MultiplexerNode>(module.get());
        }
        ImGui::SameLine();
        if (ImGui::Button("Adder Node", ImVec2(150, 150))) {
            new_node = std::make_shared<AdderNode>(module.get());
        }
        ImGui::SameLine();

        if (ImGui::Button("LT Node", ImVec2(150, 150))) {
            new_node = std::make_shared<LiteralNode>(module.get());
        }

        ImGui::SameLine();

        if (ImGui::Button("Splitter Node", ImVec2(150, 150))) {
            new_node = std::make_shared<SplitterNode>(module.get());
        }
        ImGui::SameLine();

        if (ImGui::Button("Combiner Node", ImVec2(150, 150))) {
            new_node = std::make_shared<CombinerNode>(module.get());
        }
        ImGui::SameLine();

        if (ImGui::Button("Counter Node", ImVec2(150, 150))) {
            new_node = std::make_shared<CounterNode>(module.get());
        }
        ImGui::SameLine();

        if (ImGui::Button("Register Node", ImVec2(150, 150))) {
            new_node = std::make_shared<RegisterNode>(module.get());
        }
        ImGui::SameLine();
        if (ImGui::Button("DFF Node", ImVec2(150, 150))) {
            new_node = std::make_shared<DFFNode>(module.get());
        }
        ImGui::SameLine();
        if (ImGui::Button("Debounce Node", ImVec2(150, 150))) {
            new_node = std::make_shared<DebounceNode>(module.get());
        }
        ImGui::SameLine();
        if (ImGui::Button("Edge Node", ImVec2(150, 150))) {
            new_node = std::make_shared<EdgeNode>(module.get());
        }
        ImGui::SameLine();

        if (ImGui::Button("Clock Node " ICON_FA_WAVE_SQUARE, ImVec2(150, 150))) {
            new_node = std::make_shared<ClockNode>(module.get());
        }
        ImGui::SameLine();
        if (ImGui::Button("Not Node", ImVec2(150, 150))) {
            new_node = std::make_shared<NotNode>(module.get());
        }

        if (new_node) {
            config_manager->ConfigureAndAdd(module, new_node);
        }
    }
    ImGui::EndChild();

    {
        if (ImGui::Button("Print Circuit")) {
            TraversePrint v;
            module->nodes[0]->accept(v, 0);
        }

        ImGui::SameLine();

        if (ImGui::Button("Export Circuit")) {
            Codegen c(error_manager);

            c.GenerateCode(module);
        }

        ImGui::SameLine();

        if (ImGui::Button("Save Circuit")) {
            CircuitSerializer::SaveModule(module, "../Project/circuit.json");
        }

        ImGui::SameLine();

        if (ImGui::Button("Load Circuit")) {
            module = CircuitSerializer::LoadModule("../Project/circuit.json");
        }

        ImGui::SameLine();
    }

    ImGui::End();
}
