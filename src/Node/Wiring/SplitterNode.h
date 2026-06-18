//
// Created by gabed on 4/14/2026.
//

#pragma once
#include "Node/ConfigurableBitWidthNode.h"

static auto SPLITTER_IN_PIN_VAL = "Value";

class SplitterNode final : public ConfigurableBitWidthNode {
public:
    [[nodiscard]] std::shared_ptr<Node> Clone() const override;
    [[nodiscard]] std::string GetSerializationType() const override { return "SplitterNode"; }
    [[nodiscard]] ImVec4 GetUIColor() const override { return COLOR; }
    [[nodiscard]] int GetNodeWidth() const override { return 150; }
    [[nodiscard]] bool IsSequential() const override { return false; }

    void accept(Visitor &v, int output_slot) override;


    static constexpr ImVec4 COLOR = {0.325f, 0.290f, 0.718f, 1.0f};


    static std::string GetBitOutPinName(int n);

    // Pre-configured
    SplitterNode(Module *module, const std::string &guid, int bit_width);
    // New node
    explicit SplitterNode(Module *module) : ConfigurableBitWidthNode(module, "Splitter") {}


    void InitPinsAfterConfig() override;


    Pin GetInputPin();
    Pin GetBitOutputPin(int i);
};
