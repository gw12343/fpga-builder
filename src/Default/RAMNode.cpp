//
// Created by Gabe on 5/21/2026.
//

#include "RamNode.h"
#include "GUID.h"

void RAMNode::accept(Visitor &v, const int output_slot) { v.visit(*this, output_slot); }

std::shared_ptr<Node> RAMNode::Clone() const {
    return std::make_unique<RAMNode>(module, GUID::generate_guid(), data_bits, select_bits);
}

RAMNode::RAMNode(Module *module, const std::string &guid, const int data_bits, const int select_bits) :
    ConfigurableDataAndSelectBitWidthNode(guid, module, "RAM", data_bits, select_bits) {
    InitPinsAfterConfig();
}

RAMNode::RAMNode(Module *module) : ConfigurableDataAndSelectBitWidthNode(module, "RAM") {}
void RAMNode::InitPinsAfterConfig() {
    int n = 0;
    // Inputs
    pins.push_back((Pin){RAM_IN_PIN_ADDRESS, ax::NodeEditor::PinKind::Input, *this, n++, PinDataType(select_bits)});
    pins.push_back((Pin){RAM_IN_PIN_IN, ax::NodeEditor::PinKind::Input, *this, n++, PinDataType(data_bits)});
    pins.push_back((Pin){RAM_IN_PIN_LOAD, ax::NodeEditor::PinKind::Input, *this, n++, PinDataType(1)});
    pins.push_back((Pin){RAM_IN_PIN_CLOCK, ax::NodeEditor::PinKind::Input, *this, n++, PinDataType(1)});

    // Output
    pins.push_back((Pin){"Value", ax::NodeEditor::PinKind::Output, *this, n, PinDataType(data_bits)});
}

std::string RAMNode::GetDisplayName() const {
    return name + " " + std::to_string(static_cast<int>(powl(2.0, select_bits))) + "x" + std::to_string(data_bits);
}

Pin RAMNode::GetAddressPin() { return FindPin(RAM_IN_PIN_ADDRESS).value(); }
Pin RAMNode::GetInPin() { return FindPin(RAM_IN_PIN_IN).value(); }
Pin RAMNode::GetLoadPin() { return FindPin(RAM_IN_PIN_LOAD).value(); }
Pin RAMNode::GetClockPin() { return FindPin(RAM_IN_PIN_CLOCK).value(); }
