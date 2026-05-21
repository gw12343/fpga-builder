//
// Created by Gabe on 5/21/2026.
//

#include "RegisterNode.h"
#include "GUID.h"

void RegisterNode::accept(Visitor &v, const int output_slot) { v.visit(*this, output_slot); }

std::shared_ptr<Node> RegisterNode::Clone() const {
    return std::make_unique<RegisterNode>(module, GUID::generate_guid(), bits);
}

RegisterNode::RegisterNode(Module *module, const std::string &guid, const int bit_width) :
    ConfigurableBitWidthNode(guid, module, "Register", bit_width) {
    InitPinsAfterConfig();
}

void RegisterNode::InitPinsAfterConfig() {
    int n = 0;
    // Inputs
    pins.push_back((Pin){REGISTER_IN_PIN_ENABLE, ax::NodeEditor::PinKind::Input, *this, n++, PinDataType(1)});
    pins.push_back((Pin){REGISTER_IN_PIN_CLOCK, ax::NodeEditor::PinKind::Input, *this, n++, PinDataType(1)});
    pins.push_back((Pin){REGISTER_IN_PIN_RESET, ax::NodeEditor::PinKind::Input, *this, n++, PinDataType(1)});
    pins.push_back((Pin){REGISTER_IN_PIN_D, ax::NodeEditor::PinKind::Input, *this, n++, PinDataType(bits)});

    // Outputs
    pins.push_back((Pin){"Q", ax::NodeEditor::PinKind::Output, *this, n, PinDataType(bits)});
}

Pin RegisterNode::GetEnablePin() { return FindPin(REGISTER_IN_PIN_ENABLE).value(); }
Pin RegisterNode::GetClkPin() { return FindPin(REGISTER_IN_PIN_CLOCK).value(); }
Pin RegisterNode::GetDPin() { return FindPin(REGISTER_IN_PIN_D).value(); }
Pin RegisterNode::GetResetPin() { return FindPin(REGISTER_IN_PIN_RESET).value(); }
