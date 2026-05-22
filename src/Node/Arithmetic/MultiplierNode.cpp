//
// Created by Gabe on 5/21/2026.
//

#include "MultiplierNode.h"
#include "GUID.h"

void MultiplierNode::accept(Visitor &v, const int output_slot) { v.visit(*this, output_slot); }

std::shared_ptr<Node> MultiplierNode::Clone() const {
    return std::make_unique<MultiplierNode>(module, GUID::generate_guid(), bits);
}

MultiplierNode::MultiplierNode(Module *module, const std::string &guid, const int bit_width) :
    ConfigurableBitWidthNode(guid, module, "Multiplier", bit_width) {
    InitPinsAfterConfig();
}
MultiplierNode::MultiplierNode(Module *module) : ConfigurableBitWidthNode(module, "Multiplier") {}

void MultiplierNode::InitPinsAfterConfig() {
    int n = 0;
    // Inputs
    pins.push_back((Pin){MULTIPLIER_IN_PIN_A, ax::NodeEditor::PinKind::Input, *this, n++, PinDataType(bits)});
    pins.push_back((Pin){MULTIPLIER_IN_PIN_B, ax::NodeEditor::PinKind::Input, *this, n++, PinDataType(bits)});

    // Outputs
    pins.push_back((Pin){MULTIPLIER_OUT_PIN_Q, ax::NodeEditor::PinKind::Output, *this, n, PinDataType(bits)});
}

Pin MultiplierNode::GetAInputPin() { return FindPin(MULTIPLIER_IN_PIN_A).value(); }
Pin MultiplierNode::GetBInputPin() { return FindPin(MULTIPLIER_IN_PIN_B).value(); }
