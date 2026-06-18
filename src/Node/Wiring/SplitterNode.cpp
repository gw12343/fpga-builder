//
// Created by Gabe on 5/21/2026.
//

#include "SplitterNode.h"

#include "GUID.h"


std::shared_ptr<Node> SplitterNode::Clone() const {
    return std::make_unique<SplitterNode>(module, GUID::generate_guid(), bits);
}

// Pre-configured node
SplitterNode::SplitterNode(Module *module, const std::string &guid, const int bit_width) :
    ConfigurableBitWidthNode(guid, module, "Splitter", bit_width) {
    InitPinsAfterConfig();
}
// New node
void SplitterNode::accept(Visitor &v, const int output_slot) { v.visit(*this, output_slot); }


void SplitterNode::InitPinsAfterConfig() {
    // Inputs
    pins.push_back({ SPLITTER_IN_PIN_VAL, ax::NodeEditor::PinKind::Input, *this, 0, PinDataType(bits) });

    // Outputs
    int n = 1;
    for (int i = 0; i < bits; i++) {
        Pin new_output(GetBitOutPinName(i), ax::NodeEditor::PinKind::Output, *this, n++, PinDataType(1));
        pins.push_back(new_output);
    }
}
std::string SplitterNode::GetBitOutPinName(const int n) { return "Bit " + std::to_string(n); }
Pin SplitterNode::GetInputPin() { return FindPin(SPLITTER_IN_PIN_VAL).value(); }
Pin SplitterNode::GetBitOutputPin(const int i) { return FindPin(GetBitOutPinName(i)).value(); }
