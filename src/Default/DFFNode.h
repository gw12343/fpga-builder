//
// Created by gabed on 4/14/2026.
//

#pragma once
#include "GUID.h"
#include "Node.h"


static auto IN_PIN_SET = "Set";
static auto IN_PIN_RESET = "Reset";
static auto IN_PIN_D = "D";
static auto IN_PIN_CLOCK = "Clk";

class DFFNode final : public Node {
public:
    [[nodiscard]] std::string GetSerializationType() const override { return "DFFNode"; }

    void accept(Visitor &v, const int output_slot) override { v.visit(*this, output_slot); }

    static constexpr ImVec4 color = {0.729f, 0.455f, 0.067f, 1.0f};
    [[nodiscard]] ImVec4 GetUIColor() const override { return color; }


    explicit DFFNode(Module *module) : DFFNode(module, GUID::generate_guid()) {}

    DFFNode(Module *module, const std::string &guid) :
        Node(guid, module, "D Flip Flop",
             {{IN_PIN_SET, PinDataType(1)},
              {IN_PIN_RESET, PinDataType(1)},
              {IN_PIN_D, PinDataType(1)},
              {IN_PIN_CLOCK, PinDataType(1)}},
             {{"Value", PinDataType(1)}}) {}

    Pin GetSetPin() { return FindPin(IN_PIN_SET).value(); }
    Pin GetResetPin() { return FindPin(IN_PIN_RESET).value(); }
    Pin GetDPin() { return FindPin(IN_PIN_D).value(); }
    Pin GetClkPin() { return FindPin(IN_PIN_CLOCK).value(); }
};
