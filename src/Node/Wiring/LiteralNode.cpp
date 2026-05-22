//
// Created by Gabe on 5/21/2026.
//

#include "LiteralNode.h"

#include "GUID.h"

void LiteralNode::accept(Visitor &v, const int output_slot) { v.visit(*this, output_slot); }

std::shared_ptr<Node> LiteralNode::Clone() const {
    return std::make_unique<LiteralNode>(module, GUID::generate_guid(), bits, value);
}

nlohmann::json LiteralNode::ToJson() const {
    nlohmann::json j = ConfigurableBitWidthNode::ToJson();
    j["value"] = value;
    return j;
}

void LiteralNode::RenderInternals() {
    ImGui::PushItemWidth(60);
    ImGui::SliderInt(("##" + guid).c_str(), &value, 0, powl(2, bits) - 1);
    if (value < 0) {
        value = 0;
    } else if (value > powl(2, bits) - 1) {
        value = powl(2, bits) - 1;
    }

    ImGui::PopItemWidth();
}

LiteralNode::LiteralNode(Module *module, const std::string &guid, const int bit_width, const int val) :
    ConfigurableBitWidthNode(guid, module, "Literal", bit_width), value(val) {

    InitPinsAfterConfig();
}
LiteralNode::LiteralNode(Module *module) : ConfigurableBitWidthNode(module, "Literal"), value(0) {}

void LiteralNode::InitPinsAfterConfig() {
    // Output
    pins.push_back((Pin){"Value", ax::NodeEditor::PinKind::Output, *this, 0, PinDataType(bits)});
}
