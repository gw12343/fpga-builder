//
// Created by Gabe on 5/21/2026.
//

#include "ClockNode.h"
#include "GUID.h"

void ClockNode::accept(Visitor &v, const int output_slot) { v.visit(*this, output_slot); }

std::shared_ptr<Node> ClockNode::Clone() const { return std::make_unique<ClockNode>(module, GUID::generate_guid()); }

ClockNode::ClockNode(Module *module) : ClockNode(module, GUID::generate_guid()) {}
ClockNode::ClockNode(Module *module, const std::string &guid) :
    Node(guid, module, ICON_FA_WAVE_SQUARE, {}, {{"clk", PinDataType(1)}}) {}
