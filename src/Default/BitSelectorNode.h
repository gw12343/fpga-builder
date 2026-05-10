//
// Created by gabed on 4/14/2026.
//

#pragma once
#include "ConfigurableBitRangeNode.h"
#include "GUID.h"
#include "Node.h"

static auto BITSELECTOR_IN_PIN_INPUT = "In";

static auto BITSELECTOR_OUT_PIN_OUTPUT = "Out";

class BitSelectorNode final : public ConfigurableBitRangeNode {
public:
    [[nodiscard]] std::string GetSerializationType() const override { return "BitSelectorNode"; }

    void accept(Visitor &v, const int output_slot) override { v.visit(*this, output_slot); }

    [[nodiscard]] std::shared_ptr<Node> Clone() const override {
        return std::make_unique<BitSelectorNode>(module, GUID::generate_guid(), input_width, range_start_bit,
                                                 range_end_bit);
    }

    static constexpr ImVec4 COLOR = {0.560f, 0.1f, 0.07f, 1.0f};
    [[nodiscard]] ImVec4 GetUIColor() const override { return COLOR; }


    // Pre-configured
    BitSelectorNode(Module *module, const std::string &guid, const int bit_width, const int start, const int end) :
        ConfigurableBitRangeNode(guid, module, "Bit Selector", bit_width, start, end) {
        InitPinsAfterConfig();
    }

    explicit BitSelectorNode(Module *module) : ConfigurableBitRangeNode(module, "Bit Selector") {}

    void InitPinsAfterConfig() override {
        int n = 0;
        // Inputs
        pins.push_back(
                (Pin){BITSELECTOR_IN_PIN_INPUT, ax::NodeEditor::PinKind::Input, *this, n++, PinDataType(input_width)});

        // Outputs
        pins.push_back((Pin){BITSELECTOR_OUT_PIN_OUTPUT, ax::NodeEditor::PinKind::Output, *this, n,
                             PinDataType(GetDataRangeWidth())});
    }


    Pin GetInputPin() { return FindPin(BITSELECTOR_IN_PIN_INPUT).value(); }
};
