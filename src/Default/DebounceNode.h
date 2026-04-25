//
// Created by gabed on 4/14/2026.
//

#pragma once
#include "Node.h"


static auto DEBOUNCE_IN_PIN_D = "D";
static auto DEBOUNCE_IN_PIN_CLK = "Clk";

class DebounceNode final : public Node {
public:
    [[nodiscard]] std::string GetSerializationType() const override { return "DebounceNode"; }

    void accept(Visitor &v, const int output_slot) override { v.visit(*this, output_slot); }

    [[nodiscard]] int GetNodeWidth() const override { return 100; }
    [[nodiscard]] ImVec4 GetUIColor() const override { return {0.729f, 0.455f, 0.067f, 1.0f}; }


    DebounceNode(Module *module, const std::string &guid) :
        Node(guid, module, "Debounce", {{DEBOUNCE_IN_PIN_D, PinDataType(1)}, {DEBOUNCE_IN_PIN_CLK, PinDataType(1)}},
             {{"Q", PinDataType(1)}}) {}

    Pin GetDPin() { return FindPin(DEBOUNCE_IN_PIN_D).value(); }
    Pin GetClockPin() { return FindPin(DEBOUNCE_IN_PIN_CLK).value(); }
};
