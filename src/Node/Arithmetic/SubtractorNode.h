//
// Created by gabed on 4/14/2026.
//

#pragma once

#include "Node/ConfigurableBitWidthNode.h"


static auto SUBTRACTOR_IN_PIN_A = "A";
static auto SUBTRACTOR_IN_PIN_B = "B";

static auto SUBTRACTOR_OUT_PIN_Q = "Q";
static auto SUBTRACTOR_OUT_PIN_COUT = "Carry Out";

class SubtractorNode final : public ConfigurableBitWidthNode {
public:
    [[nodiscard]] std::string GetSerializationType() const override { return "SubtractorNode"; }
    [[nodiscard]] bool IsSequential() const override { return false; }

    void accept(Visitor &v, int output_slot) override;

    static constexpr ImVec4 COLOR = {0.560f, 0.1f, 0.07f, 1.0f};
    [[nodiscard]] ImVec4 GetUIColor() const override { return COLOR; }

    [[nodiscard]] std::shared_ptr<Node> Clone() const override;

    // Pre-configured
    SubtractorNode(Module *module, const std::string &guid, int bit_width);
    explicit SubtractorNode(Module *module) : ConfigurableBitWidthNode(module, "Subtractor") {}

    void InitPinsAfterConfig() override;

    Pin GetAInputPin();
    Pin GetBInputPin();

    int SUBTRACTOR_Q_ID = -1;
    int SUBTRACTOR_COUT_ID = -1;
};
