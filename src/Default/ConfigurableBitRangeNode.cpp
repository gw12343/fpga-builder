//
// Created by Gabe on 5/21/2026.
//

#include "ConfigurableBitRangeNode.h"

// Constructor for new node
ConfigurableBitRangeNode::ConfigurableBitRangeNode(Module *parent, const std::string &name) : Node(parent, name) {
    input_width = defaultBitWidthToUse;
    range_start_bit = defaultStartBitToUse;
    range_end_bit = defaultEndBitToUse;
}
// Constructor for saved node
ConfigurableBitRangeNode::ConfigurableBitRangeNode(const std::string &saved_guid, Module *parent,
                                                   const std::string &name, const int width, const int start_bit,
                                                   const int end_bit) :
    Node(saved_guid, parent, name) {
    input_width = width;
    range_start_bit = start_bit;
    range_end_bit = end_bit;
}


// Save bit widths to json
[[nodiscard]] nlohmann::json ConfigurableBitRangeNode::ToJson() const {
    nlohmann::json j = Node::ToJson();
    j["input_width"] = input_width;
    j["range_start_bit"] = range_start_bit;
    j["range_end_bit"] = range_end_bit;
    return j;
}

// Override configuration methods
void ConfigurableBitRangeNode::RenderConfiguration() {
    ImGui::SeparatorText("Input");
    if (ImGui::DragInt("# Bits", &input_width, 1, 1, 16)) {
        defaultBitWidthToUse = input_width;
    }
    ImGui::SeparatorText("Range");
    if (ImGui::DragInt("Start Bit", &range_start_bit, 1, 0, input_width - 1)) {
        defaultStartBitToUse = range_start_bit;
    }
    if (ImGui::DragInt("Range Bit", &range_end_bit, 1, 0, input_width - 1)) {
        defaultEndBitToUse = range_end_bit;
    }
}

[[nodiscard]] std::string ConfigurableBitRangeNode::GetDisplayName() const {
    return name + " [" + std::to_string(range_end_bit) + ":" + std::to_string(range_start_bit) + "]";
}

[[nodiscard]] int ConfigurableBitRangeNode::GetDataRangeWidth() const {
    return labs(range_end_bit - range_start_bit) + 1;
}
