//
// Created by Gabe on 5/21/2026.
//

#include "MultiplexerNode.h"

#include "GUID.h"
void MultiplexerNode::accept(Visitor &v, const int output_slot) { v.visit(*this, output_slot); }
std::shared_ptr<Node> MultiplexerNode::Clone() const {
    return std::make_unique<MultiplexerNode>(module, GUID::generate_guid(), data_bits, select_bits);
}
MultiplexerNode::MultiplexerNode(Module *module, const std::string &guid, const int data_bits, const int select_bits) :
    ConfigurableDataAndSelectBitWidthNode(guid, module, "Multiplexer", data_bits, select_bits) {
    InitPinsAfterConfig();
}
MultiplexerNode::MultiplexerNode(Module *module) : ConfigurableDataAndSelectBitWidthNode(module, "Multiplexer") {}
void MultiplexerNode::InitPinsAfterConfig() {
    // Inputs
    int n = 0;
    for (int i = 0; i < GetNumOptions(); i++) {
        Pin new_input("In " + std::to_string(n), ax::NodeEditor::PinKind::Input, *this, n++, PinDataType(data_bits));
        pins.push_back(new_input);
    }
    pins.push_back({ MP_IN_PIN_SELECT, ax::NodeEditor::PinKind::Input, *this, n++, PinDataType(select_bits) });

    // Output
    pins.push_back({ "Value", ax::NodeEditor::PinKind::Output, *this, n, PinDataType(data_bits) });
}
int MultiplexerNode::GetNumOptions() const { return static_cast<int>(powl(2.0, select_bits)); }
Pin MultiplexerNode::GetInputPin(const int n) { return pins[n]; }
Pin MultiplexerNode::GetSelectInputPin() { return FindPin(MP_IN_PIN_SELECT).value(); }
