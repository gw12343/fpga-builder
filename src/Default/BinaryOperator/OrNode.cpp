//
// Created by Gabe on 5/21/2026.
//

#include "OrNode.h"
#include "GUID.h"

std::string OrNode::GetVerilogAssign(const std::string &out, const std::vector<std::string> &inputs) const {
    std::string assign = out + " = " + inputs[0];
    for (int i = 1; i < inputs_size; i++) {
        assign += " | " + inputs[i];
    }
    return assign + ";\n";
}

void OrNode::accept(Visitor &v, const int output_slot) { v.visit(*this, output_slot); }

std::shared_ptr<Node> OrNode::Clone() const {
    return std::make_unique<OrNode>(module, GUID::generate_guid(), data_bits, inputs_size);
}

OrNode::OrNode(Module *module, const std::string &guid, const int bit_width, const int num_inputs) :
    BinaryOpNode(module, guid, "OR", bit_width, num_inputs) {}
OrNode::OrNode(Module *parent) : BinaryOpNode(parent, "OR") {}
