//
// Created by gabed on 4/14/2026.
//

#include "InputTunnelNode.h"

#include "Events/NumericParameterChangeCommand.h"
#include "GUID.h"
#include "Module.h"


std::shared_ptr<Node> InputTunnelNode::Clone() const {
    return std::make_unique<InputTunnelNode>(module, GUID::generate_guid(), net_name, data_bits);
}


void InputTunnelNode::PushStyle() {
    PushStyleColor(ax::NodeEditor::StyleColor_NodeBg, ImVec4(163 / 255.0, 176 / 255.0, 199 / 255.0, 255 / 255.0));
    PushStyleColor(ax::NodeEditor::StyleColor_NodeBorder, ImVec4(0, 0, 0, 255 / 255.0));
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0, 0, 0, 255 / 255.0));
}

void InputTunnelNode::PopStyle() {
    ImGui::PopStyleColor(1);
    ax::NodeEditor::PopStyleColor(2);
}

InputTunnelNode::InputTunnelNode(Module *module, const std::string &guid, std::string net, int bits) :
    ConfigurableTunnelNode(guid, module, "Input Tunnel", bits, net) {
    InitPinsAfterConfig();
}


InputTunnelNode::InputTunnelNode(Module *module) : ConfigurableTunnelNode(module, "Input Tunnel") {}


void InputTunnelNode::InitPinsAfterConfig() {
    pins.emplace_back(GetTunnelPinName(), ax::NodeEditor::PinKind::Input, *this, 0, PinDataType(data_bits));
}


void InputTunnelNode::accept(Visitor &v, const int output_slot) { v.visit(*this, output_slot); }

Pin InputTunnelNode::GetInputPin() { return FindPin(GetTunnelPinName()).value(); }
