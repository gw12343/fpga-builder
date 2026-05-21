//
// Created by gabed on 4/14/2026.
//

#pragma once
#include "Node.h"


static auto EDGE_IN_PIN_D = "D";
static auto EDGE_IN_PIN_CLK = "Clk";

static auto EDGE_OUT_PIN_Q = "Q";
static auto EDGE_OUT_PIN_NQ = "!Q";

class EdgeNode : public Node {
public:
    [[nodiscard]] std::shared_ptr<Node> Clone() const override;
    [[nodiscard]] std::string GetSerializationType() const override { return "EdgeNode"; }
    [[nodiscard]] int GetNodeWidth() const override { return 100; }
    [[nodiscard]] ImVec4 GetUIColor() const override { return COLOR; }

    void accept(Visitor &v, int output_slot) override;


    EdgeNode(Module *module, const std::string &guid);

    explicit EdgeNode(Module *module);

    Pin GetDPin();
    Pin GetClockPin();


    const int EDGE_OUT_Q_ID;
    const int EDGE_OUT_NQ_ID;

    static constexpr ImVec4 COLOR = {0.729f, 0.455f, 0.067f, 1.0f};
};
