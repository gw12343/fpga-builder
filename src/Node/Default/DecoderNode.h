//
// Created by gabed on 4/14/2026.
//

#pragma once
#include "Node/ConfigurableBitWidthNode.h"

static auto DECODER_IN_PIN_VAL = "Value";


class DecoderNode final : public ConfigurableBitWidthNode {
public:
    [[nodiscard]] std::string GetSerializationType() const override { return "DecoderNode"; }
    [[nodiscard]] std::shared_ptr<Node> Clone() const override;
    [[nodiscard]] ImVec4 GetUIColor() const override { return COLOR; }
    [[nodiscard]] int GetNodeWidth() const override { return 150; }

    // Pre-configured
    DecoderNode(Module *module, const std::string &guid, int bit_width);
    // New node
    explicit DecoderNode(Module *module);


    void accept(Visitor &v, int output_slot) override;

    static std::string GetBitOutPinName(int n);
    void InitPinsAfterConfig() override;
    int GetNumOutputs() const;


    Pin GetInputPin();
    Pin GetBitOutputPin(int i);

    static constexpr ImVec4 COLOR = {0.325f, 0.290f, 0.718f, 1.0f};
};
