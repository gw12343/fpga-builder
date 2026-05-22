//
// Created by Gabe on 5/21/2026.
//

#include "DFFNode.h"

#include "GUID.h"


DFFNode::DFFNode(Module *module) : DFFNode(module, GUID::generate_guid()) {}

DFFNode::DFFNode(Module *module, const std::string &guid) :
    Node(guid, module, "D Flip Flop",
         {{DFF_IN_PIN_SET, PinDataType(1)},
          {DFF_IN_PIN_RESET, PinDataType(1)},
          {DFF_IN_PIN_D, PinDataType(1)},
          {DFF_IN_PIN_CLOCK, PinDataType(1)}},
         {{"Value", PinDataType(1)}}) {}

std::shared_ptr<Node> DFFNode::Clone() const { return std::make_unique<DFFNode>(module, GUID::generate_guid()); }
void DFFNode::accept(Visitor &v, const int output_slot) { v.visit(*this, output_slot); }


Pin DFFNode::GetSetPin() { return FindPin(DFF_IN_PIN_SET).value(); }
Pin DFFNode::GetResetPin() { return FindPin(DFF_IN_PIN_RESET).value(); }
Pin DFFNode::GetDPin() { return FindPin(DFF_IN_PIN_D).value(); }
Pin DFFNode::GetClkPin() { return FindPin(DFF_IN_PIN_CLOCK).value(); }
