//
// Created by Gabe on 5/21/2026.
//

#include "DebounceNode.h"
#include "GUID.h"

void DebounceNode::accept(Visitor &v, const int output_slot) { v.visit(*this, output_slot); }
std::shared_ptr<Node> DebounceNode::Clone() const {
    return std::make_unique<DebounceNode>(module, GUID::generate_guid());
}

DebounceNode::DebounceNode(Module *module, const std::string &guid) :
    Node(guid, module, "Debounce", {{DEBOUNCE_IN_PIN_D, PinDataType(1)}, {DEBOUNCE_IN_PIN_CLK, PinDataType(1)}},
         {{"Q", PinDataType(1)}}) {}

DebounceNode::DebounceNode(Module *module) : DebounceNode(module, GUID::generate_guid()) {}


Pin DebounceNode::GetDPin() { return FindPin(DEBOUNCE_IN_PIN_D).value(); }
Pin DebounceNode::GetClockPin() { return FindPin(DEBOUNCE_IN_PIN_CLK).value(); }
