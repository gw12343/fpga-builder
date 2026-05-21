//
// Created by Gabe on 5/21/2026.
//

#include "BinaryOpNode.h"
#include "Default/Node.h"
#include "Pins/Pin.h"


BinaryOpNode::BinaryOpNode(Module *module, const std::string &guid, const std::string &name, const int bit_width,
                           const int num_inputs) :
    ConfigurableDataAndNumInputsNode(guid, module, name, bit_width, num_inputs) {
    BinaryOpNode::InitPinsAfterConfig();
}

void BinaryOpNode::accept(Visitor &v, const int output_slot) { v.visit(*this, output_slot); }

std::string BinaryOpNode::GetBitInPinName(const int n) { return "Input " + std::to_string(n); }

Pin BinaryOpNode::GetInputPin(const int n) { return FindPin(GetBitInPinName(n)).value(); }


void BinaryOpNode::InitPinsAfterConfig() {
    int n = 0;
    // Inputs
    for (int i = 0; i < inputs_size; i++) {
        Pin new_input(GetBitInPinName(i), ax::NodeEditor::PinKind::Input, *this, n++, PinDataType(data_bits));
        pins.push_back(new_input);
    }

    // Output
    pins.push_back((Pin){"Out", ax::NodeEditor::PinKind::Output, *this, n, PinDataType(data_bits)});
}
