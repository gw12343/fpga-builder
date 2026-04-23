//
// Created by gabed on 4/14/2026.
//

#pragma once
#include "Node.h"


static auto COUTNER_IN_PIN_ENABLE = "Enable";
static auto COUTNER_IN_PIN_CLOCK = "Clk";
static auto COUTNER_IN_PIN_RESET = "Reset";
static auto COUTNER_IN_PIN_COUNT_UP = "Count Up";

class CounterNode final : public Node {
public:
    [[nodiscard]] std::string GetSerializationType() const override { return "CounterNode"; }

    void accept(Visitor &v, const int output_slot) override { v.visit(*this, output_slot); }

    [[nodiscard]] nlohmann::json ToJson() const override {
        nlohmann::json j = Node::ToJson();
        j["bits"] = bits;
        return j;
    }

    [[nodiscard]] ImVec4 GetUIColor() const override { return {0.729f, 0.455f, 0.067f, 1.0f}; }
    [[nodiscard]] int GetNodeWidth() const override { return 150; }


    CounterNode(Module *module, const std::string &guid, const int bit_width) :
        Node(guid, module, "Counter",
             {{COUTNER_IN_PIN_ENABLE, PinDataType(1)},
              {COUTNER_IN_PIN_CLOCK, PinDataType(1)},
              {COUTNER_IN_PIN_COUNT_UP, PinDataType(1)},
              {COUTNER_IN_PIN_RESET, PinDataType(1)}},
             {{"Value", PinDataType(bit_width)}}) {
        bits = bit_width;
    }


    int bits;

    Pin GetEnablePin() { return FindPin(COUTNER_IN_PIN_ENABLE).value(); }
    Pin GetCountUpPin() { return FindPin(COUTNER_IN_PIN_COUNT_UP).value(); }
    Pin GetClkPin() { return FindPin(COUTNER_IN_PIN_CLOCK).value(); }
    Pin GetResetPin() { return FindPin(COUTNER_IN_PIN_RESET).value(); }
};
