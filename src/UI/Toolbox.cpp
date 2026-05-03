//
// Created by Gabe on 4/26/2026.
//

#include "Toolbox.h"

#include "CircuitSerializer.h"
#include "Codegen/Codegen.h"
#include "Default/AdderNode.h"
#include "Default/BinaryOperator/AndNode.h"
#include "Default/BinaryOperator/NandNode.h"
#include "Default/BinaryOperator/NorNode.h"
#include "Default/BinaryOperator/OrNode.h"
#include "Default/BinaryOperator/XOrNode.h"
#include "Default/ClockNode.h"
#include "Default/CombinerNode.h"
#include "Default/CounterNode.h"
#include "Default/CustomModuleNode.h"
#include "Default/DFFNode.h"
#include "Default/DebounceNode.h"
#include "Default/DecoderNode.h"
#include "Default/EdgeNode.h"
#include "Default/InputNode.h"
#include "Default/LiteralNode.h"
#include "Default/MultiplexerNode.h"
#include "Default/OutputNode.h"
#include "Default/RegisterNode.h"
#include "Default/SplitterNode.h"
#include "Default/SubtractorNode.h"
#include "Default/UnaryOperator/NotNode.h"
#include "Lib/IconsFontAwesome6.h"


#define NODE_MIN_BTN_SIZE 150

#define ADD_NODE_TO_CATEGORY(category, name, type)                                                                     \
                                                                                                                       \
    m_categories[category].push_back({name, type::COLOR, [](const std::shared_ptr<Module> &module) {                   \
                                          return std::make_shared<type>(module.get());                                 \
                                      }});

Toolbox::Toolbox() {
    ADD_NODE_TO_CATEGORY("Bitwise Operators", "Custom", CustomModuleNode);
    ADD_NODE_TO_CATEGORY("Bitwise Operators", "NOT", NotNode);
    ADD_NODE_TO_CATEGORY("Bitwise Operators", "AND", AndNode);
    ADD_NODE_TO_CATEGORY("Bitwise Operators", "NAND", NandNode);
    ADD_NODE_TO_CATEGORY("Bitwise Operators", "OR", OrNode);
    ADD_NODE_TO_CATEGORY("Bitwise Operators", "NOR", NorNode);
    ADD_NODE_TO_CATEGORY("Bitwise Operators", "XOR", XOrNode);

    ADD_NODE_TO_CATEGORY("Wiring", "Literal", LiteralNode);
    ADD_NODE_TO_CATEGORY("Wiring", "Splitter", SplitterNode);
    ADD_NODE_TO_CATEGORY("Wiring", "Combiner", CombinerNode);
    ADD_NODE_TO_CATEGORY("Wiring", "Multiplexer", MultiplexerNode);
    ADD_NODE_TO_CATEGORY("Wiring", "Decoder", DecoderNode);

    ADD_NODE_TO_CATEGORY("IO", "Input", InputNode);
    ADD_NODE_TO_CATEGORY("IO", "Output", OutputNode);


    ADD_NODE_TO_CATEGORY("Memory", "Register", RegisterNode);
    ADD_NODE_TO_CATEGORY("Memory", "Counter", CounterNode);
    ADD_NODE_TO_CATEGORY("Memory", "DFF", DFFNode);

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


    // NODE_BTN("NOT", NotNode);
    //
    // NODE_BTN("OR", OrNode);
    // NODE_BTN("NOR", NorNode);
    // NODE_BTN("AND", AndNode);
    // NODE_BTN("XOR", XOrNode);

    // NODE_BTN("MUX", MultiplexerNode);
    // NODE_BTN("SPLITTER", SplitterNode);
    // NODE_BTN("COMBINER", CombinerNode);
    // NODE_BTN("#", LiteralNode);


    // NODE_BTN("COUNTER", CounterNode);
    // NODE_BTN("DFF", DFFNode);
    // NODE_BTN("REGISTER", RegisterNode);

    // NODE_BTN("ADDER", AdderNode);

    // NODE_BTN("DEBOUNCE", DebounceNode);
    // NODE_BTN("EDGE", EdgeNode);
    // NODE_BTN(ICON_FA_WAVE_SQUARE, ClockNode);

    if (new_node) {
        if (module.has_value())
            config_manager->ConfigureAndAdd(module.value(), new_node);
    }


    ImGui::End();
}
