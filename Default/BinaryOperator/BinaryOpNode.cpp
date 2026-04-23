//
// Created by gabed on 4/15/2026.
//

#include "BinaryOpNode.h"

#define IN_PIN_A "A"
#define IN_PIN_B "B"


BinaryOpNode::BinaryOpNode(Module *parent, const std::string &guid) :
    Node(guid, parent, "Binary op", {{IN_PIN_A, PinDataType(4)}, {IN_PIN_B, PinDataType(4)}},
         {{"Out", PinDataType(4)}}) {}


Pin BinaryOpNode::GetAInputPin() { return FindPin(IN_PIN_A).value(); }

Pin BinaryOpNode::GetBInputPin() { return FindPin(IN_PIN_B).value(); }
