//
// Created by gabed on 4/14/2026.
//

#pragma once
#include "ConfigurableBitWidthNode.h"
#include "Node.h"

static auto DECODER_IN_PIN_VAL = "Value";


class DecoderNode final : public ConfigurableBitWidthNode {
public:
    [[nodiscard]] std::string GetSerializationType() const override { return "DecoderNode"; }

    void accept(Visitor &v, const int output_slot) override { v.visit(*this, output_slot); }

    [[nodiscard]] std::shared_ptr<Node> Clone() const override {
        return std::make_unique<DecoderNode>(module, GUID::generate_guid(), bits);
    }

    static constexpr ImVec4 COLOR = {0.325f, 0.290f, 0.718f, 1.0f};
    [[nodiscard]] ImVec4 GetUIColor() const override { return COLOR; }
    [[nodiscard]] int GetNodeWidth() const override { return 150; }


    static std::string GetBitOutPinName(const int n) { return "Out " + std::to_string(n); }

    // Pre-configured
    DecoderNode(Module *module, const std::string &guid, const int bit_width) :
        ConfigurableBitWidthNode(guid, module, "Decoder", bit_width) {
        InitPinsAfterConfig();
    }

    explicit DecoderNode(Module *module) : ConfigurableBitWidthNode(module, "Decoder") {}


    [[nodiscard]] int GetNumOutputs() const { return static_cast<int>(powl(2.0, bits)); }


    void InitPinsAfterConfig() override {
        // Inputs
        pins.push_back((Pin){DECODER_IN_PIN_VAL, ax::NodeEditor::PinKind::Input, *this, 0, PinDataType(bits)});

        // Outputs
        int n = 1;
        for (int i = 0; i < GetNumOutputs(); i++) {
            Pin new_output(GetBitOutPinName(i), ax::NodeEditor::PinKind::Output, *this, n++, PinDataType(1));
            pins.push_back(new_output);
        }
    }


    Pin GetInputPin() { return FindPin(DECODER_IN_PIN_VAL).value(); }
    Pin GetBitOutputPin(const int i) { return FindPin(GetBitOutPinName(i)).value(); }
};
