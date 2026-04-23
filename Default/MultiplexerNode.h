//
// Created by gabed on 4/14/2026.
//

#pragma once
#include "Node.h"

static auto IN_PIN_A = "A";
static auto IN_PIN_B = "B";
static auto IN_PIN_SELECT = "Sel";

class MultiplexerNode final : public Node {
public:
    [[nodiscard]] std::string GetSerializationType() const override { return "MultiplexerNode"; }

    void accept(Visitor &v, const int output_slot) override { v.visit(*this, output_slot); }

    [[nodiscard]] ImVec4 GetUIColor() const override { return {0.325f, 0.290f, 0.718f, 1.0f}; }


    MultiplexerNode(Module *module, const std::string &guid) :
        Node(guid, module, "Multiplexer",
             {{IN_PIN_A, PinDataType(1)}, {IN_PIN_B, PinDataType(1)}, {IN_PIN_SELECT, PinDataType(1)}},
             {{"Value", PinDataType(1)}}) {}

    Pin GetAInputPin() { return FindPin(IN_PIN_A).value(); }
    Pin GetBInputPin() { return FindPin(IN_PIN_B).value(); }
    Pin GetSelectInputPin() { return FindPin(IN_PIN_SELECT).value(); }
};
