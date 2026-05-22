//
// Created by Gabe on 5/21/2026.
//

#include "SubtractorNode.h"
#include "GUID.h"

void SubtractorNode::accept(Visitor &v, const int output_slot) { v.visit(*this, output_slot); }

[[nodiscard]] std::shared_ptr<Node> SubtractorNode::Clone() const {
    return std::make_unique<SubtractorNode>(module, GUID::generate_guid(), bits);
}

// Pre-configured
SubtractorNode::SubtractorNode(Module *module, const std::string &guid, const int bit_width) :
    ConfigurableBitWidthNode(guid, module, "Subtractor", bit_width) {
    InitPinsAfterConfig();
}


void SubtractorNode::InitPinsAfterConfig() {
    int n = 0;
    // Inputs
    pins.push_back((Pin){SUBTRACTOR_IN_PIN_A, ax::NodeEditor::PinKind::Input, *this, n++, PinDataType(bits)});
    pins.push_back((Pin){SUBTRACTOR_IN_PIN_B, ax::NodeEditor::PinKind::Input, *this, n++, PinDataType(bits)});

    // Outputs
    pins.push_back((Pin){SUBTRACTOR_OUT_PIN_Q, ax::NodeEditor::PinKind::Output, *this, n++, PinDataType(bits)});
    pins.push_back((Pin){SUBTRACTOR_OUT_PIN_COUT, ax::NodeEditor::PinKind::Output, *this, n, PinDataType(1)});

    // Find output pin ids
    SUBTRACTOR_Q_ID = FindPin(SUBTRACTOR_OUT_PIN_Q).value().GetNodeIndex();
    SUBTRACTOR_COUT_ID = FindPin(SUBTRACTOR_OUT_PIN_COUT).value().GetNodeIndex();
}

Pin SubtractorNode::GetAInputPin() { return FindPin(SUBTRACTOR_IN_PIN_A).value(); }
Pin SubtractorNode::GetBInputPin() { return FindPin(SUBTRACTOR_IN_PIN_B).value(); }
