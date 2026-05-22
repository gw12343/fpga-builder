//
// Created by gabed on 4/14/2026.
//

#pragma once

#include "Node/ConfigurableBitWidthNode.h"

static auto COMBINER_OUT_PIN_VAL = "Value";


class CombinerNode final : public ConfigurableBitWidthNode {
public:
    [[nodiscard]] std::shared_ptr<Node> Clone() const override;
    [[nodiscard]] std::string GetSerializationType() const override { return "CombinerNode"; }
    [[nodiscard]] ImVec4 GetUIColor() const override { return COLOR; }
    [[nodiscard]] int GetNodeWidth() const override { return 150; }

    CombinerNode(Module *module, const std::string &guid, int data_width);
    explicit CombinerNode(Module *module);

    void InitPinsAfterConfig() override;
    void accept(Visitor &v, int output_slot) override;

    static std::string GetBitInPinName(int n);
    Pin GetBitInputPin(const int i) { return FindPin(GetBitInPinName(i)).value(); }

    static constexpr ImVec4 COLOR = {0.325f, 0.290f, 0.718f, 1.0f};
};
