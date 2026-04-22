//
// Created by gabed on 4/14/2026.
//

#pragma once
#include "Node.h"


static auto DEBOUNCE_IN_PIN_D = "D";
static auto DEBOUNCE_IN_PIN_CLK = "Clk";

class DebounceNode final : public Node {
public:
    [[nodiscard]] std::string type() const override { return "DebounceNode"; }

    void accept(Visitor &v, const int output_slot) override { v.visit(*this, output_slot); }

    [[nodiscard]] int width() const override { return 100; }
    [[nodiscard]] ImVec4 color() const override { return {0.729f, 0.455f, 0.067f, 1.0f}; }


    DebounceNode(Module *module, const std::string &guid) :
        Node(guid, module, "Debounce", {DEBOUNCE_IN_PIN_D, DEBOUNCE_IN_PIN_CLK}, {"Q"}) {}

    Pin GetDPin() { return FindPin(DEBOUNCE_IN_PIN_D).value(); }
    Pin GetClockPin() { return FindPin(DEBOUNCE_IN_PIN_CLK).value(); }
};
