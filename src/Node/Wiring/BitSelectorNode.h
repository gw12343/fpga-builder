//
// Created by gabed on 4/14/2026.
//

#pragma once
#include "Node/ConfigurableBitRangeNode.h"

static auto BITSELECTOR_IN_PIN_INPUT = "In";

static auto BITSELECTOR_OUT_PIN_OUTPUT = "Out";

class BitSelectorNode final : public ConfigurableBitRangeNode {
public:
    [[nodiscard]] std::shared_ptr<Node> Clone() const override;
    [[nodiscard]] std::string GetSerializationType() const override { return "BitSelectorNode"; }
    [[nodiscard]] ImVec4 GetUIColor() const override { return COLOR; }
    [[nodiscard]] bool IsSequential() const override { return false; }

    // Pre-configured
    BitSelectorNode(Module *module, const std::string &guid, const int bit_width, const int start, const int end);
    // New node
    explicit BitSelectorNode(Module *module);

    void InitPinsAfterConfig() override;
    void accept(Visitor &v, const int output_slot) override;

    Pin GetInputPin();

    static constexpr ImVec4 COLOR = {0.325f, 0.290f, 0.718f, 1.0f};
};
