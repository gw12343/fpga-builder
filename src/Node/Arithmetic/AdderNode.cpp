//
// Created by Gabe on 5/21/2026.
//

#include "AdderNode.h"
#include "GUID.h"

void AdderNode::accept(Visitor &v, const int output_slot) { v.visit(*this, output_slot); }
std::shared_ptr<Node> AdderNode::Clone() const {
    return std::make_unique<AdderNode>(module, GUID::generate_guid(), bits);
}
AdderNode::AdderNode(Module *module, const std::string &guid, const int bit_width) :
    ConfigurableBitWidthNode(guid, module, "Adder", bit_width) {
    InitPinsAfterConfig();
}
AdderNode::AdderNode(Module *module) : ConfigurableBitWidthNode(module, "Adder") {}
void AdderNode::InitPinsAfterConfig() {
    int n = 0;
    // Inputs
    pins.push_back({ADDER_IN_PIN_A, ax::NodeEditor::PinKind::Input, *this, n++, PinDataType(bits)});
    pins.push_back({ADDER_IN_PIN_B, ax::NodeEditor::PinKind::Input, *this, n++, PinDataType(bits)});
    pins.push_back({ADDER_IN_PIN_CIN, ax::NodeEditor::PinKind::Input, *this, n++, PinDataType(1)});

    // Outputs
    pins.push_back({ADDER_OUT_PIN_Q, ax::NodeEditor::PinKind::Output, *this, n++, PinDataType(bits)});
    pins.push_back({ADDER_OUT_PIN_COUT, ax::NodeEditor::PinKind::Output, *this, n, PinDataType(1)});

    // Find output pin ids
    ADDER_Q_ID = FindPin(ADDER_OUT_PIN_Q).value().GetNodeIndex();
    ADDER_COUT_ID = FindPin(ADDER_OUT_PIN_COUT).value().GetNodeIndex();
}
Pin AdderNode::GetAInputPin() { return FindPin(ADDER_IN_PIN_A).value(); }
Pin AdderNode::GetBInputPin() { return FindPin(ADDER_IN_PIN_B).value(); }
Pin AdderNode::GetCarryInputPin() { return FindPin(ADDER_IN_PIN_CIN).value(); }
