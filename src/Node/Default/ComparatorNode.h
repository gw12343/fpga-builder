//
// Created by gabed on 4/14/2026.
//

#pragma once

#include "Node/ConfigurableBitWidthNode.h"


static auto COMPARATOR_IN_PIN_A = "A";
static auto COMPARATOR_IN_PIN_B = "B";

static auto COMPARATOR_OUT_PIN_GREATER = ">";
static auto COMPARATOR_OUT_PIN_EQUAL = "=";
static auto COMPARATOR_OUT_PIN_LESS = "<";

class ComparatorNode final : public ConfigurableBitWidthNode {
public:
    [[nodiscard]] std::shared_ptr<Node> Clone() const override;
    [[nodiscard]] std::string GetSerializationType() const override { return "ComparatorNode"; }
    [[nodiscard]] ImVec4 GetUIColor() const override { return COLOR; }
    [[nodiscard]] bool IsSequential() const override { return false; }

    // Pre-configured
    ComparatorNode(Module *module, const std::string &guid, const int bit_width);

    explicit ComparatorNode(Module *module);

    void InitPinsAfterConfig() override;
    void accept(Visitor &v, const int output_slot) override;

    Pin GetAInputPin();
    Pin GetBInputPin();

    int COMPARATOR_G_ID = -1;
    int COMPARATOR_E_ID = -1;
    int COMPARATOR_L_ID = -1;
    static constexpr ImVec4 COLOR = {0.560f, 0.1f, 0.07f, 1.0f};
};
