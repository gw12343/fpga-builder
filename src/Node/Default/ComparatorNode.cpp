//
// Created by Gabe on 5/21/2026.
//

#include "ComparatorNode.h"
#include "GUID.h"

void ComparatorNode::accept(Visitor &v, const int output_slot) { v.visit(*this, output_slot); }

std::shared_ptr<Node> ComparatorNode::Clone() const {
    return std::make_unique<ComparatorNode>(module, GUID::generate_guid(), bits);
}

ComparatorNode::ComparatorNode(Module *module, const std::string &guid, const int bit_width) :
    ConfigurableBitWidthNode(guid, module, "Comparator", bit_width) {
    InitPinsAfterConfig();
}

ComparatorNode::ComparatorNode(Module *module) : ConfigurableBitWidthNode(module, "Comparator") {}

void ComparatorNode::InitPinsAfterConfig() {
    int n = 0;
    // Inputs
    pins.push_back({ COMPARATOR_IN_PIN_A, ax::NodeEditor::PinKind::Input, *this, n++, PinDataType(bits) });
    pins.push_back({ COMPARATOR_IN_PIN_B, ax::NodeEditor::PinKind::Input, *this, n++, PinDataType(bits) });

    // Outputs
    pins.push_back({ COMPARATOR_OUT_PIN_GREATER, ax::NodeEditor::PinKind::Output, *this, n++, PinDataType(1) });
    pins.push_back({ COMPARATOR_OUT_PIN_EQUAL, ax::NodeEditor::PinKind::Output, *this, n++, PinDataType(1) });
    pins.push_back({ COMPARATOR_OUT_PIN_LESS, ax::NodeEditor::PinKind::Output, *this, n, PinDataType(1) });

    // Find output pin ids
    COMPARATOR_G_ID = FindPin(COMPARATOR_OUT_PIN_GREATER).value().GetNodeIndex();
    COMPARATOR_E_ID = FindPin(COMPARATOR_OUT_PIN_EQUAL).value().GetNodeIndex();
    COMPARATOR_L_ID = FindPin(COMPARATOR_OUT_PIN_LESS).value().GetNodeIndex();
}

Pin ComparatorNode::GetAInputPin() { return FindPin(COMPARATOR_IN_PIN_A).value(); }
Pin ComparatorNode::GetBInputPin() { return FindPin(COMPARATOR_IN_PIN_B).value(); }
