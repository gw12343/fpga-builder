//
// Created by gabed on 4/14/2026.
//

#pragma once
#include "ConfigurableBitWidthNode.h"

static auto SPLITTER_IN_PIN_VAL = "Value";

class SplitterNode final : public ConfigurableBitWidthNode {
public:
    [[nodiscard]] std::string GetSerializationType() const override { return "SplitterNode"; }

    void accept(Visitor &v, int output_slot) override;

    [[nodiscard]] std::shared_ptr<Node> Clone() const override;

    static constexpr ImVec4 COLOR = {0.325f, 0.290f, 0.718f, 1.0f};
    [[nodiscard]] ImVec4 GetUIColor() const override { return COLOR; }
    [[nodiscard]] int GetNodeWidth() const override { return 150; }


    static std::string GetBitOutPinName(int n);

    // Pre-configured
    SplitterNode(Module *module, const std::string &guid, int bit_width);
    // New node
    explicit SplitterNode(Module *module) : ConfigurableBitWidthNode(module, "Splitter") {}


    void InitPinsAfterConfig() override;


    Pin GetInputPin();
    Pin GetBitOutputPin(int i);
};
