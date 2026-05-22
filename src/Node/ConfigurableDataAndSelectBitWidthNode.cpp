//
// Created by Gabe on 5/21/2026.
//

#include "ConfigurableDataAndSelectBitWidthNode.h"

// Constructor for new node
ConfigurableDataAndSelectBitWidthNode::ConfigurableDataAndSelectBitWidthNode(Module *parent, const std::string &name) :
    Node(parent, name) {
    data_bits = defaultDataBitsToUse;
    select_bits = defaultSelectBitsToUse;
}
// Constructor for saved node
ConfigurableDataAndSelectBitWidthNode::ConfigurableDataAndSelectBitWidthNode(const std::string &saved_guid,
                                                                             Module *parent, const std::string &name,
                                                                             const int data_width,
                                                                             const int select_width) :
    Node(saved_guid, parent, name) {
    data_bits = data_width;
    select_bits = select_width;
}


// Save bit widths to json
[[nodiscard]] nlohmann::json ConfigurableDataAndSelectBitWidthNode::ToJson() const {
    nlohmann::json j = Node::ToJson();
    j["data_bits"] = data_bits;
    j["select_bits"] = select_bits;
    return j;
}

// Override configuration methods
void ConfigurableDataAndSelectBitWidthNode::RenderConfiguration() {
    if (ImGui::DragInt("# Data Bits", &data_bits, 1, 1, 16)) {
        defaultDataBitsToUse = data_bits;
    }
    if (ImGui::DragInt("# Select Bits", &select_bits, 1, 1, 4)) {
        defaultSelectBitsToUse = select_bits;
    }
}

[[nodiscard]] std::string ConfigurableDataAndSelectBitWidthNode::GetDisplayName() const {
    return name + " (" + std::to_string(static_cast<int>(powl(2, select_bits))) + "-to-1 x" +
           std::to_string(data_bits) + ")";
}
