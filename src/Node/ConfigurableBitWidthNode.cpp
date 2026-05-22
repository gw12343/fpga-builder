//
// Created by Gabe on 5/21/2026.
//

#include "ConfigurableBitWidthNode.h"

// Constructor for new node
ConfigurableBitWidthNode::ConfigurableBitWidthNode(Module *parent, const std::string &name) : Node(parent, name) {
    bits = defaultBitsToUse;
}
// Constructor for saved node
ConfigurableBitWidthNode::ConfigurableBitWidthNode(const std::string &saved_guid, Module *parent,
                                                   const std::string &name, const int data_bits) :
    Node(saved_guid, parent, name) {
    bits = data_bits;
}


// Save bit width to json
[[nodiscard]] nlohmann::json ConfigurableBitWidthNode::ToJson() const {
    nlohmann::json j = Node::ToJson();
    j["bits"] = bits;
    return j;
}

// Override configuration methods
void ConfigurableBitWidthNode::RenderConfiguration() {
    if (ImGui::DragInt("# Data Bits", &bits, 1, 1, 16)) {
        defaultBitsToUse = bits;
    }
}

[[nodiscard]] std::string ConfigurableBitWidthNode::GetDisplayName() const {
    return name + " (" + std::to_string(bits) + "bit)";
}
