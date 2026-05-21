//
// Created by Gabe on 5/21/2026.
//

#include "ConfigurableDataAndNumInputsNode.h"

// Constructor for new node
ConfigurableDataAndNumInputsNode::ConfigurableDataAndNumInputsNode(Module *parent, const std::string &name) :
    Node(parent, name) {
    data_bits = defaultDataBitsToUse;
    inputs_size = defaultNumInputs;
}
// Constructor for saved node
ConfigurableDataAndNumInputsNode::ConfigurableDataAndNumInputsNode(const std::string &saved_guid, Module *parent,
                                                                   const std::string &name, const int data_width,
                                                                   const int num_inputs) :
    Node(saved_guid, parent, name) {
    data_bits = data_width;
    inputs_size = num_inputs;
}


[[nodiscard]] nlohmann::json ConfigurableDataAndNumInputsNode::ToJson() const {
    nlohmann::json j = Node::ToJson();
    j["bits"] = data_bits;
    j["num_inputs"] = inputs_size;
    return j;
}

void ConfigurableDataAndNumInputsNode::RenderConfiguration() {
    if (ImGui::DragInt("# Data Bits", &data_bits, 1, 1, 16)) {
        defaultDataBitsToUse = data_bits;
    }
    if (ImGui::DragInt("# Inputs", &inputs_size, 1, 2, 10)) {
        defaultNumInputs = inputs_size;
    }
}

[[nodiscard]] std::string ConfigurableDataAndNumInputsNode::GetDisplayName() const {
    return name + " (" + std::to_string(data_bits) + ")";
}
