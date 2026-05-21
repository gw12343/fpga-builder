//
// Created by Gabe on 5/21/2026.
//

#include "NotNode.h"

#include "GUID.h"
std::string NotNode::GetVerilogAssign(const std::string &out, const std::string &a) const {
    return out + " = ~" + a + ";\n";
}
std::shared_ptr<Node> NotNode::Clone() const { return std::make_unique<NotNode>(module, GUID::generate_guid(), bits); }
NotNode::NotNode(Module *module, const std::string &guid, const int bit_width) :
    UnaryOpNode(module, guid, "NOT", bit_width) {}
NotNode::NotNode(Module *parent) : UnaryOpNode(parent, "NOT") {}
void NotNode::accept(Visitor &v, const int output_slot) { v.visit(*this, output_slot); }
