//
// Created by gabed on 4/14/2026.
//

#pragma once
#include "Node.h"

static auto ADDER_IN_PIN_A = "A";
static auto ADDER_IN_PIN_B = "B";
static auto ADDER_IN_PIN_CIN = "Carry In";

static auto ADDER_OUT_PIN_Q = "Q";
static auto ADDER_OUT_PIN_COUT = "Carry Out";

class AdderNode final : public Node {
public:
    [[nodiscard]] std::string GetSerializationType() const override { return "AdderNode"; }

    void accept(Visitor &v, const int output_slot) override { v.visit(*this, output_slot); }

    [[nodiscard]] nlohmann::json ToJson() const override {
        nlohmann::json j = Node::ToJson();
        j["bits"] = bits;
        return j;
    }

    [[nodiscard]] ImVec4 GetUIColor() const override { return {0.560f, 0.1f, 0.07f, 1.0f}; }


    AdderNode(Module *module, const std::string &guid, const int bit_width) :
        Node(guid, module, "Adder",
             {{ADDER_IN_PIN_A, PinDataType(bit_width)},
              {ADDER_IN_PIN_B, PinDataType(bit_width)},
              {ADDER_IN_PIN_CIN, PinDataType(1)}},
             {{ADDER_OUT_PIN_Q, PinDataType(bit_width)}, {ADDER_OUT_PIN_COUT, PinDataType(1)}}),
        ADDER_Q_ID(FindPin(ADDER_OUT_PIN_Q).value().GetNodeIndex()),
        ADDER_COUT_ID(FindPin(ADDER_OUT_PIN_COUT).value().GetNodeIndex()) {
        bits = bit_width;
    }


    int bits;

    const int ADDER_Q_ID;
    const int ADDER_COUT_ID;

    Pin GetAInputPin() { return FindPin(ADDER_IN_PIN_A).value(); }
    Pin GetBInputPin() { return FindPin(ADDER_IN_PIN_B).value(); }
    Pin GetCarryInputPin() { return FindPin(ADDER_IN_PIN_CIN).value(); }
};
