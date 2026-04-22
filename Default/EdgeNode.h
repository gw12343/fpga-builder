//
// Created by gabed on 4/14/2026.
//

#pragma once
#include "Node.h"


#include <iostream>

static auto EDGE_IN_PIN_D = "D";
static auto EDGE_IN_PIN_CLK = "Clk";

static auto EDGE_OUT_PIN_Q = "Q";
static auto EDGE_OUT_PIN_NQ = "!Q";

class EdgeNode final : public Node {
public:
    [[nodiscard]] std::string type() const override { return "EdgeNode"; }

    void accept(Visitor &v, const int output_slot) override { v.visit(*this, output_slot); }

    [[nodiscard]] int width() const override { return 100; }
    [[nodiscard]] ImVec4 color() const override { return {0.729f, 0.455f, 0.067f, 1.0f}; }

    EdgeNode(Module *module, const std::string &guid) :
        Node(guid, module, "Edge", {EDGE_IN_PIN_D, EDGE_IN_PIN_CLK}, {EDGE_OUT_PIN_Q, EDGE_OUT_PIN_NQ}),

        EDGE_OUT_Q_ID(FindPin(EDGE_OUT_PIN_Q).value().GetNodeIndex()),
        EDGE_OUT_NQ_ID(FindPin(EDGE_OUT_PIN_NQ).value().GetNodeIndex()) {}

    Pin GetDPin() { return FindPin(EDGE_IN_PIN_D).value(); }
    Pin GetClockPin() { return FindPin(EDGE_IN_PIN_CLK).value(); }

    const int EDGE_OUT_Q_ID;
    const int EDGE_OUT_NQ_ID;
};
