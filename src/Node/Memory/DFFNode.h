//
// Created by gabed on 4/14/2026.
//

#pragma once
#include "Node/Node.h"


static auto DFF_IN_PIN_SET = "Set";
static auto DFF_IN_PIN_RESET = "Reset";
static auto DFF_IN_PIN_D = "D";
static auto DFF_IN_PIN_CLOCK = "Clk";

class DFFNode final : public Node {
public:
    [[nodiscard]] std::shared_ptr<Node> Clone() const override;
    [[nodiscard]] std::string GetSerializationType() const override { return "DFFNode"; }
    [[nodiscard]] ImVec4 GetUIColor() const override { return COLOR; }
    [[nodiscard]] bool IsSequential() const override { return true; }

    explicit DFFNode(Module *module);
    DFFNode(Module *module, const std::string &guid);

    void accept(Visitor &v, int output_slot) override;

    Pin GetSetPin();
    Pin GetResetPin();
    Pin GetDPin();
    Pin GetClkPin();

    static constexpr ImVec4 COLOR = {0.729f, 0.455f, 0.067f, 1.0f};
};
