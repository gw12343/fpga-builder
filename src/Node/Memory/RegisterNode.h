//
// Created by gabed on 4/14/2026.
//

#pragma once
#include "Node/ConfigurableBitWidthNode.h"


static auto REGISTER_IN_PIN_ENABLE = "Enable";
static auto REGISTER_IN_PIN_CLOCK = "Clk";
static auto REGISTER_IN_PIN_RESET = "Reset";
static auto REGISTER_IN_PIN_D = "D";

class RegisterNode final : public ConfigurableBitWidthNode {
public:
    [[nodiscard]] std::shared_ptr<Node> Clone() const override;
    [[nodiscard]] std::string GetSerializationType() const override { return "RegisterNode"; }
    [[nodiscard]] int GetNodeWidth() const override { return 150; }
    [[nodiscard]] ImVec4 GetUIColor() const override { return COLOR; }
    [[nodiscard]] bool IsSequential() const override { return true; }

    void accept(Visitor &v, int output_slot) override;
    void InitPinsAfterConfig() override;

    // Pre-configured node
    RegisterNode(Module *module, const std::string &guid, int bit_width);
    // New node
    explicit RegisterNode(Module *module) : ConfigurableBitWidthNode(module, "Register") {}


    Pin GetEnablePin();
    Pin GetClkPin();
    Pin GetDPin();
    Pin GetResetPin();


    static constexpr ImVec4 COLOR = {0.729f, 0.455f, 0.067f, 1.0f};
};
