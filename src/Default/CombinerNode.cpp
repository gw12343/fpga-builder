//
// Created by Gabe on 5/21/2026.
//

#include "CombinerNode.h"

#include "GUID.h"
void CombinerNode::accept(Visitor &v, const int output_slot) { v.visit(*this, output_slot); }

std::shared_ptr<Node> CombinerNode::Clone() const {
    return std::make_unique<CombinerNode>(module, GUID::generate_guid(), bits);
}

std::string CombinerNode::GetBitInPinName(const int n) { return "Bit " + std::to_string(n); }

CombinerNode::CombinerNode(Module *module, const std::string &guid, const int data_width) :
    ConfigurableBitWidthNode(guid, module, "Combiner", data_width) {
    InitPinsAfterConfig();
}

CombinerNode::CombinerNode(Module *module) : ConfigurableBitWidthNode(module, "Combiner") {}

void CombinerNode::InitPinsAfterConfig() {
    // Output
    pins.push_back((Pin){COMBINER_OUT_PIN_VAL, ax::NodeEditor::PinKind::Output, *this, 0, PinDataType(bits)});
    // Inputs
    int n = 1;
    for (int i = 0; i < bits; i++) {
        Pin new_input(GetBitInPinName(i), ax::NodeEditor::PinKind::Input, *this, n++, PinDataType(1));
        pins.push_back(new_input);
    }
}
