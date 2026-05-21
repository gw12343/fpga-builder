//
// Created by Gabe on 5/21/2026.
//

#include "UnaryOpNode.h"
void UnaryOpNode::accept(Visitor &v, const int output_slot) { v.visit(*this, output_slot); }
std::string UnaryOpNode::GetVerilogAssign(const std::string &out, const std::string &a) const { return "????"; }


UnaryOpNode::UnaryOpNode(Module *module, const std::string &guid, const std::string &name, const int bit_width) :
    ConfigurableBitWidthNode(guid, module, name, bit_width) {
    UnaryOpNode::InitPinsAfterConfig();
}
UnaryOpNode::UnaryOpNode(Module *module, const std::string &name) : ConfigurableBitWidthNode(module, name) {}

void UnaryOpNode::InitPinsAfterConfig() {
    // Input
    pins.push_back((Pin){UNARY_OP_IN_PIN_A, ax::NodeEditor::PinKind::Input, *this, 0, PinDataType(bits)});
    // Output
    pins.push_back((Pin){"Out", ax::NodeEditor::PinKind::Output, *this, 1, PinDataType(bits)});
}

Pin UnaryOpNode::GetAInputPin() { return FindPin(UNARY_OP_IN_PIN_A).value(); }
