//
// Created by gabed on 4/14/2026.
//

#pragma once
#include "Node/ConfigurableBitWidthNode.h"

static auto ADDER_IN_PIN_A = "A";
static auto ADDER_IN_PIN_B = "B";
static auto ADDER_IN_PIN_CIN = "Carry In";

static auto ADDER_OUT_PIN_Q = "Q";
static auto ADDER_OUT_PIN_COUT = "Carry Out";

class AdderNode final : public ConfigurableBitWidthNode {
public:
    [[nodiscard]] std::shared_ptr<Node> Clone() const override;
    [[nodiscard]] std::string GetSerializationType() const override { return "AdderNode"; }
    [[nodiscard]] ImVec4 GetUIColor() const override { return COLOR; }

    // Pre-configured
    AdderNode(Module *module, const std::string &guid, const int bit_width);
    // New node
    explicit AdderNode(Module *module);

    void InitPinsAfterConfig() override;

    void accept(Visitor &v, int output_slot) override;

    Pin GetAInputPin();
    Pin GetBInputPin();
    Pin GetCarryInputPin();

    int ADDER_Q_ID = -1;
    int ADDER_COUT_ID = -1;

    static constexpr ImVec4 COLOR = {0.560f, 0.1f, 0.07f, 1.0f};
};
