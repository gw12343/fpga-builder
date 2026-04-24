//
// Created by gabed on 4/14/2026.
//

#pragma once
#include "Node.h"


static auto REGISTER_IN_PIN_ENABLE = "Enable";
static auto REGISTER_IN_PIN_CLOCK = "Clk";
static auto REGISTER_IN_PIN_RESET = "Reset";
static auto REGISTER_IN_PIN_D = "D";

class RegisterNode final : public Node {
public:
    [[nodiscard]] std::string GetSerializationType() const override { return "RegisterNode"; }

    void accept(Visitor &v, const int output_slot) override { v.visit(*this, output_slot); }

    [[nodiscard]] nlohmann::json ToJson() const override {
        nlohmann::json j = Node::ToJson();
        j["bits"] = bits;
        return j;
    }

    [[nodiscard]] ImVec4 GetUIColor() const override { return {0.729f, 0.455f, 0.067f, 1.0f}; }
    [[nodiscard]] int GetNodeWidth() const override { return 150; }


    RegisterNode(Module *module, const std::string &guid, const int bit_width) :
        Node(guid, module, "Register",
             {{REGISTER_IN_PIN_ENABLE, PinDataType(1)},
              {REGISTER_IN_PIN_CLOCK, PinDataType(1)},
              {REGISTER_IN_PIN_D, PinDataType(bit_width)},
              {REGISTER_IN_PIN_RESET, PinDataType(1)}},
             {{"Q", PinDataType(bit_width)}}) {
        bits = bit_width;
    }


    int bits;

    Pin GetEnablePin() { return FindPin(REGISTER_IN_PIN_ENABLE).value(); }
    Pin GetClkPin() { return FindPin(REGISTER_IN_PIN_CLOCK).value(); }
    Pin GetDPin() { return FindPin(REGISTER_IN_PIN_D).value(); }
    Pin GetResetPin() { return FindPin(REGISTER_IN_PIN_RESET).value(); }
};
