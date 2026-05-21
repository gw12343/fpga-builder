//
// Created by Gabe on 5/21/2026.
//

#include "BitSelectorNode.h"
#include "GUID.h"

void BitSelectorNode::accept(Visitor &v, const int output_slot) { v.visit(*this, output_slot); }
std::shared_ptr<Node> BitSelectorNode::Clone() const {
    return std::make_unique<BitSelectorNode>(module, GUID::generate_guid(), input_width, range_start_bit,
                                             range_end_bit);
}
BitSelectorNode::BitSelectorNode(Module *module, const std::string &guid, const int bit_width, const int start,
                                 const int end) :
    ConfigurableBitRangeNode(guid, module, "Bit Selector", bit_width, start, end) {
    InitPinsAfterConfig();
}
BitSelectorNode::BitSelectorNode(Module *module) : ConfigurableBitRangeNode(module, "Bit Selector") {}
void BitSelectorNode::InitPinsAfterConfig() {
    int n = 0;
    // Inputs
    pins.push_back(
            (Pin){BITSELECTOR_IN_PIN_INPUT, ax::NodeEditor::PinKind::Input, *this, n++, PinDataType(input_width)});

    // Outputs
    pins.push_back((Pin){BITSELECTOR_OUT_PIN_OUTPUT, ax::NodeEditor::PinKind::Output, *this, n,
                         PinDataType(GetDataRangeWidth())});
}
Pin BitSelectorNode::GetInputPin() { return FindPin(BITSELECTOR_IN_PIN_INPUT).value(); }
