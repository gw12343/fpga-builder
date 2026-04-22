//
// Created by gabed on 4/14/2026.
//

#pragma once
#include "Node.h"


static auto IN_PIN_SET = "Set";
static auto IN_PIN_RESET = "Reset";
static auto IN_PIN_D = "D";
static auto IN_PIN_CLOCK = "Clk";

class DFFNode final : public Node {
public:
    [[nodiscard]] std::string type() const override { return "DFFNode"; }

    void accept(Visitor &v, const int output_slot) override { v.visit(*this, output_slot); }

    [[nodiscard]] ImVec4 color() const override { return {0.729f, 0.455f, 0.067f, 1.0f}; }


    DFFNode(Module *module, const std::string &guid) :
        Node(guid, module, "D Flip Flop", {IN_PIN_SET, IN_PIN_RESET, IN_PIN_D, IN_PIN_CLOCK}, {"Value"}) {}

    Pin GetSetPin() { return FindPin(IN_PIN_SET).value(); }
    Pin GetResetPin() { return FindPin(IN_PIN_RESET).value(); }
    Pin GetDPin() { return FindPin(IN_PIN_D).value(); }
    Pin GetClkPin() { return FindPin(IN_PIN_CLOCK).value(); }
};
