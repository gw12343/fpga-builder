//
// Created by Gabe on 5/21/2026.
//

#include "CounterNode.h"

#include "GUID.h"
void CounterNode::accept(Visitor &v, const int output_slot) { v.visit(*this, output_slot); }
std::shared_ptr<Node> CounterNode::Clone() const {
    return std::make_unique<CounterNode>(module, GUID::generate_guid(), bits);
}
CounterNode::CounterNode(Module *module, const std::string &guid, const int bit_width) :
    ConfigurableBitWidthNode(guid, module, "Counter", bit_width) {
    InitPinsAfterConfig();
}
CounterNode::CounterNode(Module *module) : ConfigurableBitWidthNode(module, "Counter") {}
void CounterNode::InitPinsAfterConfig() {
    int n = 0;
    // Inputs
    pins.push_back((Pin){COUTNER_IN_PIN_ENABLE, ax::NodeEditor::PinKind::Input, *this, n++, PinDataType(1)});
    pins.push_back((Pin){COUTNER_IN_PIN_CLOCK, ax::NodeEditor::PinKind::Input, *this, n++, PinDataType(1)});
    pins.push_back((Pin){COUTNER_IN_PIN_COUNT_UP, ax::NodeEditor::PinKind::Input, *this, n++, PinDataType(1)});
    pins.push_back((Pin){COUTNER_IN_PIN_RESET, ax::NodeEditor::PinKind::Input, *this, n++, PinDataType(1)});

    pins.push_back((Pin){COUTNER_IN_PIN_LOAD, ax::NodeEditor::PinKind::Input, *this, n++, PinDataType(1)});
    pins.push_back((Pin){COUTNER_IN_PIN_DATA, ax::NodeEditor::PinKind::Input, *this, n++, PinDataType(bits)});

    // Outputs
    pins.push_back((Pin){"Value", ax::NodeEditor::PinKind::Output, *this, n, PinDataType(bits)});
}
Pin CounterNode::GetEnablePin() { return FindPin(COUTNER_IN_PIN_ENABLE).value(); }
Pin CounterNode::GetCountUpPin() { return FindPin(COUTNER_IN_PIN_COUNT_UP).value(); }
Pin CounterNode::GetClkPin() { return FindPin(COUTNER_IN_PIN_CLOCK).value(); }
Pin CounterNode::GetResetPin() { return FindPin(COUTNER_IN_PIN_RESET).value(); }
Pin CounterNode::GetLoadPin() { return FindPin(COUTNER_IN_PIN_LOAD).value(); }
Pin CounterNode::GetLoadValuePin() { return FindPin(COUTNER_IN_PIN_DATA).value(); }
