//
// Created by gabed on 4/14/2026.
//

#pragma once
#include "ConfigurableBitWidthNode.h"


static auto MULTIPLIER_IN_PIN_A = "A";
static auto MULTIPLIER_IN_PIN_B = "B";

static auto MULTIPLIER_OUT_PIN_Q = "Q";

class MultiplierNode final : public ConfigurableBitWidthNode {
public:
    [[nodiscard]] std::string GetSerializationType() const override { return "MultiplierNode"; }
    [[nodiscard]] std::shared_ptr<Node> Clone() const override;

    // Pre-configured
    MultiplierNode(Module *module, const std::string &guid, int bit_width);
    // New node
    explicit MultiplierNode(Module *module);

    void accept(Visitor &v, int output_slot) override;
    void InitPinsAfterConfig() override;


    [[nodiscard]] ImVec4 GetUIColor() const override { return COLOR; }
    Pin GetAInputPin();
    Pin GetBInputPin();

    static constexpr ImVec4 COLOR = {0.560f, 0.1f, 0.07f, 1.0f};
};
