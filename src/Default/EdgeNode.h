//
// Created by gabed on 4/14/2026.
//

#pragma once
#include "Node.h"


static auto EDGE_IN_PIN_D = "D";
static auto EDGE_IN_PIN_CLK = "Clk";

static auto EDGE_OUT_PIN_Q = "Q";
static auto EDGE_OUT_PIN_NQ = "!Q";

class EdgeNode final : public Node {
public:
    [[nodiscard]] std::string GetSerializationType() const override { return "EdgeNode"; }

    void accept(Visitor &v, const int output_slot) override { v.visit(*this, output_slot); }

    [[nodiscard]] int GetNodeWidth() const override { return 100; }
    static constexpr ImVec4 color = {0.729f, 0.455f, 0.067f, 1.0f};
    [[nodiscard]] ImVec4 GetUIColor() const override { return color; }

    explicit EdgeNode(Module *module) : EdgeNode(module, GUID::generate_guid()) {}


    EdgeNode(Module *module, const std::string &guid) :
        Node(guid, module, "Edge", {{EDGE_IN_PIN_D, PinDataType(1)}, {EDGE_IN_PIN_CLK, PinDataType(1)}},
             {{EDGE_OUT_PIN_Q, PinDataType(1)}, {EDGE_OUT_PIN_NQ, PinDataType(1)}}),

        EDGE_OUT_Q_ID(FindPin(EDGE_OUT_PIN_Q).value().GetNodeIndex()),
        EDGE_OUT_NQ_ID(FindPin(EDGE_OUT_PIN_NQ).value().GetNodeIndex()) {}

    Pin GetDPin() { return FindPin(EDGE_IN_PIN_D).value(); }
    Pin GetClockPin() { return FindPin(EDGE_IN_PIN_CLK).value(); }

    const int EDGE_OUT_Q_ID;
    const int EDGE_OUT_NQ_ID;
};
