//
// Created by Gabe on 5/21/2026.
//

#include "AndNode.h"
#include "GUID.h"

[[nodiscard]] std::string AndNode::GetVerilogAssign(const std::string &out,
                                                    const std::vector<std::string> &inputs) const {
    std::string assign = out + " = " + inputs[0];
    for (int i = 1; i < inputs_size; i++) {
        assign += " & " + inputs[i];
    }
    return assign + ";\n";
}
void AndNode::accept(Visitor &v, const int output_slot) { v.visit(*this, output_slot); }

[[nodiscard]] std::shared_ptr<Node> AndNode::Clone() const {
    return std::make_unique<AndNode>(module, GUID::generate_guid(), data_bits, inputs_size);
}
