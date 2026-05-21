//
// Created by Gabe on 5/21/2026.
//

#include "EdgeNode.h"
#include "GUID.h"


void EdgeNode::accept(Visitor &v, const int output_slot) { v.visit(*this, output_slot); }
std::shared_ptr<Node> EdgeNode::Clone() const { return std::make_unique<EdgeNode>(module, GUID::generate_guid()); }
EdgeNode::EdgeNode(Module *module) : EdgeNode(module, GUID::generate_guid()) {}

EdgeNode::EdgeNode(Module *module, const std::string &guid) :
    Node(guid, module, "Edge", {{EDGE_IN_PIN_D, PinDataType(1)}, {EDGE_IN_PIN_CLK, PinDataType(1)}},
         {{EDGE_OUT_PIN_Q, PinDataType(1)}, {EDGE_OUT_PIN_NQ, PinDataType(1)}}),

    EDGE_OUT_Q_ID(FindPin(EDGE_OUT_PIN_Q).value().GetNodeIndex()),
    EDGE_OUT_NQ_ID(FindPin(EDGE_OUT_PIN_NQ).value().GetNodeIndex()) {}

Pin EdgeNode::GetDPin() { return FindPin(EDGE_IN_PIN_D).value(); }
Pin EdgeNode::GetClockPin() { return FindPin(EDGE_IN_PIN_CLK).value(); }
