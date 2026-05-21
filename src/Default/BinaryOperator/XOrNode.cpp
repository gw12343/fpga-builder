//
// Created by Gabe on 5/21/2026.
//

#include "XOrNode.h"

#include "GUID.h"

std::string XOrNode::GetVerilogAssign(const std::string &out, const std::vector<std::string> &inputs) const {
    std::string assign = out + " = " + inputs[0];
    for (int i = 1; i < inputs_size; i++) {
        assign += " ^ " + inputs[i];
    }
    return assign + ";\n";
}

void XOrNode::accept(Visitor &v, const int output_slot) { v.visit(*this, output_slot); }

std::shared_ptr<Node> XOrNode::Clone() const {
    return std::make_unique<XOrNode>(module, GUID::generate_guid(), data_bits, inputs_size);
}
XOrNode::XOrNode(Module *module, const std::string &guid, const int bit_width, const int num_inputs) :
    BinaryOpNode(module, guid, "XOR", bit_width, num_inputs) {}

XOrNode::XOrNode(Module *parent) : BinaryOpNode(parent, "XOR") {}
