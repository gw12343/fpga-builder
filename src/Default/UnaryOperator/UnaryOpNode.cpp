//
// Created by gabed on 4/15/2026.
//

#include "UnaryOpNode.h"

#define IN_PIN_A "A"


UnaryOpNode::UnaryOpNode(Module *parent, const std::string &guid) :
    Node(guid, parent, "Unary Op", {{IN_PIN_A, PinDataType(1)}}, {{"Out", PinDataType(1)}}) {}


Pin UnaryOpNode::GetAInputPin() { return FindPin(IN_PIN_A).value(); }
