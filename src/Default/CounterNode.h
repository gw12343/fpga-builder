//
// Created by gabed on 4/14/2026.
//

#pragma once
#include "ConfigurableBitWidthNode.h"


static auto COUTNER_IN_PIN_ENABLE = "Enable";
static auto COUTNER_IN_PIN_CLOCK = "Clk";
static auto COUTNER_IN_PIN_RESET = "Reset";
static auto COUTNER_IN_PIN_COUNT_UP = "Count Up";

static auto COUTNER_IN_PIN_LOAD = "Load Enable";
static auto COUTNER_IN_PIN_DATA = "Load Value";

class CounterNode final : public ConfigurableBitWidthNode {
public:
    [[nodiscard]] std::shared_ptr<Node> Clone() const override;
    [[nodiscard]] std::string GetSerializationType() const override { return "CounterNode"; }
    [[nodiscard]] ImVec4 GetUIColor() const override { return COLOR; }
    [[nodiscard]] int GetNodeWidth() const override { return 175; }

    // Pre-configured node
    CounterNode(Module *module, const std::string &guid, const int bit_width);
    // New node
    explicit CounterNode(Module *module);

    void InitPinsAfterConfig() override;
    void accept(Visitor &v, int output_slot) override;

    Pin GetEnablePin();
    Pin GetCountUpPin();
    Pin GetClkPin();
    Pin GetResetPin();
    Pin GetLoadPin();
    Pin GetLoadValuePin();

    static constexpr ImVec4 COLOR = {0.729f, 0.455f, 0.067f, 1.0f};
};
