//
// Created by gabed on 4/14/2026.
//

#include "OutputTunnelNode.h"

#include "Events/NumericParameterChangeCommand.h"
#include "GUID.h"
#include "Module.h"


std::shared_ptr<Node> OutputTunnelNode::Clone() const {
    return std::make_unique<OutputTunnelNode>(module, GUID::generate_guid(), net_name, data_bits);
}


void OutputTunnelNode::PushStyle() {
    PushStyleColor(ax::NodeEditor::StyleColor_NodeBg, ImVec4(163 / 255.0, 176 / 255.0, 199 / 255.0, 255 / 255.0));
    PushStyleColor(ax::NodeEditor::StyleColor_NodeBorder, ImVec4(0, 0, 0, 255 / 255.0));
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0, 0, 0, 255 / 255.0));
}

void OutputTunnelNode::PopStyle() {
    ImGui::PopStyleColor(1);
    ax::NodeEditor::PopStyleColor(2);
}

OutputTunnelNode::OutputTunnelNode(Module *module, const std::string &guid, std::string net, int bits) :
    ConfigurableTunnelNode(guid, module, "Output Tunnel", bits, net) {
    InitPinsAfterConfig();
}


OutputTunnelNode::OutputTunnelNode(Module *module) : ConfigurableTunnelNode(module, "Output Tunnel") {}


void OutputTunnelNode::InitPinsAfterConfig() {
    pins.emplace_back(GetTunnelPinName(), ax::NodeEditor::PinKind::Output, *this, 0, PinDataType(data_bits));
}


void OutputTunnelNode::accept(Visitor &v, const int output_slot) { v.visit(*this, output_slot); }
