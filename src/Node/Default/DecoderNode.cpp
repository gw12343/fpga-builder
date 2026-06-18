//
// Created by Gabe on 5/21/2026.
//

#include "DecoderNode.h"
#include "GUID.h"

std::shared_ptr<Node> DecoderNode::Clone() const {
    return std::make_unique<DecoderNode>(module, GUID::generate_guid(), bits);
}

std::string DecoderNode::GetBitOutPinName(const int n) { return "Out " + std::to_string(n); }


DecoderNode::DecoderNode(Module *module, const std::string &guid, const int bit_width) :
    ConfigurableBitWidthNode(guid, module, "Decoder", bit_width) {
    InitPinsAfterConfig();
}
DecoderNode::DecoderNode(Module *module) : ConfigurableBitWidthNode(module, "Decoder") {}

void DecoderNode::accept(Visitor &v, const int output_slot) { v.visit(*this, output_slot); }

int DecoderNode::GetNumOutputs() const { return static_cast<int>(powl(2.0, bits)); }

void DecoderNode::InitPinsAfterConfig() {
    // Inputs
    pins.push_back({ DECODER_IN_PIN_VAL, ax::NodeEditor::PinKind::Input, *this, 0, PinDataType(bits) });

    // Outputs
    int n = 1;
    for (int i = 0; i < GetNumOutputs(); i++) {
        Pin new_output(GetBitOutPinName(i), ax::NodeEditor::PinKind::Output, *this, n++, PinDataType(1));
        pins.push_back(new_output);
    }
}

Pin DecoderNode::GetInputPin() { return FindPin(DECODER_IN_PIN_VAL).value(); }

Pin DecoderNode::GetBitOutputPin(const int i) { return FindPin(GetBitOutPinName(i)).value(); }
