//
// Created by Gabe on 4/26/2026.
//

#include "Toolbox.h"

#include "../Node/BitwiseOperator/NotNode.h"
#include "../Node/BitwiseOperator/ShifterNode.h"
#include "../Node/Wiring/BitSelectorNode.h"
#include "../Node/Wiring/CombinerNode.h"
#include "../Node/Wiring/SplitterNode.h"
#include "CircuitSerializer.h"
#include "Codegen/Codegen.h"
#include "Lib/IconsFontAwesome6.h"
#include "Node/Arithmetic/AdderNode.h"
#include "Node/Arithmetic/MultiplierNode.h"
#include "Node/Arithmetic/SubtractorNode.h"
#include "Node/BitwiseOperator/AndNode.h"
#include "Node/BitwiseOperator/NandNode.h"
#include "Node/BitwiseOperator/NorNode.h"
#include "Node/BitwiseOperator/OrNode.h"
#include "Node/BitwiseOperator/XOrNode.h"
#include "Node/CustomModuleNode.h"
#include "Node/Default/ClockNode.h"
#include "Node/Default/ComparatorNode.h"
#include "Node/Default/DebounceNode.h"
#include "Node/Default/DecoderNode.h"
#include "Node/Default/EdgeNode.h"
#include "Node/IO/InputNode.h"
#include "Node/IO/OutputNode.h"
#include "Node/Memory/CounterNode.h"
#include "Node/Memory/DFFNode.h"
#include "Node/Memory/RAMNode.h"
#include "Node/Memory/ROMNode.h"
#include "Node/Memory/RegisterNode.h"
#include "Node/Wiring/InputTunnelNode.h"
#include "Node/Wiring/LiteralNode.h"
#include "Node/Wiring/MultiplexerNode.h"
#include "Node/Wiring/OutputTunnelNode.h"


#define NODE_MIN_BTN_SIZE 150

#define ADD_NODE_TO_CATEGORY(category, name, type)                                                                     \
                                                                                                                       \
    m_categories[category].push_back({name, type::COLOR, [](const std::shared_ptr<Module> &module) {                   \
                                          auto n = std::make_shared<type>(module.get());                               \
                                          n->start_pos = ImVec2(50, 50);                                               \
                                          return n;                                                                    \
                                      }});

Toolbox::Toolbox() {
    ADD_NODE_TO_CATEGORY("Bitwise Operators", "Custom", CustomModuleNode);
    ADD_NODE_TO_CATEGORY("Bitwise Operators", "NOT", NotNode);
    ADD_NODE_TO_CATEGORY("Bitwise Operators", "AND", AndNode);
    ADD_NODE_TO_CATEGORY("Bitwise Operators", "NAND", NandNode);
    ADD_NODE_TO_CATEGORY("Bitwise Operators", "OR", OrNode);
    ADD_NODE_TO_CATEGORY("Bitwise Operators", "NOR", NorNode);
    ADD_NODE_TO_CATEGORY("Bitwise Operators", "XOR", XOrNode);
    ADD_NODE_TO_CATEGORY("Bitwise Operators", "Shifter", ShifterNode);

    ADD_NODE_TO_CATEGORY("Wiring", "Literal #", LiteralNode);
    ADD_NODE_TO_CATEGORY("Wiring", "Splitter", SplitterNode);
    ADD_NODE_TO_CATEGORY("Wiring", "Combiner", CombinerNode);
    ADD_NODE_TO_CATEGORY("Wiring", "Multiplexer", MultiplexerNode);
    ADD_NODE_TO_CATEGORY("Wiring", "Decoder", DecoderNode);
    ADD_NODE_TO_CATEGORY("Wiring", "Bit Selector", BitSelectorNode);
    ADD_NODE_TO_CATEGORY("Wiring", "Tunnel In", InputTunnelNode);
    ADD_NODE_TO_CATEGORY("Wiring", "Tunnel Out", OutputTunnelNode);

    ADD_NODE_TO_CATEGORY("IO", "Input", InputNode);
    ADD_NODE_TO_CATEGORY("IO", "Output", OutputNode);
    ADD_NODE_TO_CATEGORY("IO", "ROM", ROMNode);
    ADD_NODE_TO_CATEGORY("IO", "RAM", RAMNode);


    ADD_NODE_TO_CATEGORY("Memory", "Register", RegisterNode);
    ADD_NODE_TO_CATEGORY("Memory", "Counter", CounterNode);
    ADD_NODE_TO_CATEGORY("Memory", "DFF", DFFNode);

    ADD_NODE_TO_CATEGORY("Misc", "Comparator", ComparatorNode);
    ADD_NODE_TO_CATEGORY("Misc", "Multiplier", MultiplierNode);
    ADD_NODE_TO_CATEGORY("Misc", "Subtractor", SubtractorNode);
    ADD_NODE_TO_CATEGORY("Misc", "Adder", AdderNode);
    ADD_NODE_TO_CATEGORY("Misc", "Clock " ICON_FA_WAVE_SQUARE, ClockNode);
    ADD_NODE_TO_CATEGORY("Misc", "Debounce", DebounceNode);
    ADD_NODE_TO_CATEGORY("Misc", "Edge", EdgeNode);
}


void Toolbox::Render(const std::optional<std::shared_ptr<Module>> &module,
                     const std::shared_ptr<ConfigManager> &config_manager) {
    ImGui::Begin("Toolbox");


    std::shared_ptr<Node> new_node;


    for (const auto &[category, node_types]: m_categories) {
        if (node_types.empty())
            continue;

        ImGui::SeparatorText(category.c_str());
        constexpr float NODE_WIDTH = NODE_MIN_BTN_SIZE;
        const float btn_padding = ImGui::GetStyle().ItemSpacing.x;
        const float avail_width = ImGui::GetContentRegionAvail().x;
        const int columns = std::max(1, static_cast<int>((avail_width + btn_padding) / (NODE_WIDTH + btn_padding)));

        const float adjusted_width_extra = (avail_width - columns * (NODE_WIDTH + btn_padding) + btn_padding) / columns;

        int n = 0;
        int j = 0;
        for (const auto &[name, color, creator]: node_types) {
            if (n == columns) {
                n = 0;
            } else if (j != 0) {
                ImGui::SameLine();
            }
            n++;
            j++;
            ImGui::PushStyleColor(ImGuiCol_Button, color);
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(color.x, color.y + .1, color.z + .1, color.w));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(color.x + .2, color.y + .2, color.z + .2, color.w));
            if (ImGui::Button(name.c_str(), ImVec2(NODE_MIN_BTN_SIZE + adjusted_width_extra, NODE_MIN_BTN_SIZE))) {
                if (module.has_value())
                    new_node = creator(module.value());
            }
            ImGui::PopStyleColor(3);
        }
    }


    if (new_node) {
        if (module.has_value())
            config_manager->ConfigureAndAdd(module.value(), new_node);
    }

    ImGui::End();
}
