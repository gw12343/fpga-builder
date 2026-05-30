//
// Created by Gabe on 5/26/2026.
//

#include "ConfigurableTunnelNode.h"

// Constructor for new node
ConfigurableTunnelNode::ConfigurableTunnelNode(Module *parent, const std::string &name) : Node(parent, name) {
    data_bits = defaultDataBitsToUse;
    net_name = defaultNetNameToUse;
}
// Constructor for saved node
ConfigurableTunnelNode::ConfigurableTunnelNode(const std::string &saved_guid, Module *parent, const std::string &name,
                                               const int data_width, const std::string &net) :
    Node(saved_guid, parent, name) {
    data_bits = data_width;
    net_name = net;
}


// Save bit widths to json
[[nodiscard]] nlohmann::json ConfigurableTunnelNode::ToJson() const {
    nlohmann::json j = Node::ToJson();
    j["net_name"] = net_name;
    j["data_bits"] = data_bits;
    return j;
}

// Override configuration methods
void ConfigurableTunnelNode::RenderConfiguration() {
    // TODO undo redo actions...
    if (ImGui::InputText("Net name", &net_name)) {
        defaultNetNameToUse = net_name;
    }
    if (ImGui::DragInt("# Data Bits", &data_bits, 1, 1, 16)) {
        defaultDataBitsToUse = data_bits;
    }
}

[[nodiscard]] std::string ConfigurableTunnelNode::GetDisplayName() const {
    // Show no header for this node
    return "";
}

std::string ConfigurableTunnelNode::GetTunnelPinName() const {
    return net_name + " (" + std::to_string(data_bits) + ")";
}

int ConfigurableTunnelNode::GetNodeWidth() const { return ImGui::CalcTextSize(GetTunnelPinName().c_str()).x; }
