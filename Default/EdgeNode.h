//
// Created by gabed on 4/14/2026.
//

#pragma once
#include "Node.h"


static auto EDGE_IN_PIN_D =  "D";
static auto EDGE_IN_PIN_CLK =  "Clk";

class EdgeNode : public Node {
public:

    [[nodiscard]] std::string type() const override { return "EdgeNode"; }

    void accept(Visitor& v) override { v.visit(*this); }


    EdgeNode(Module* module, const std::string &guid) :
            Node(guid, module,
                 "Edge Node",
                 {EDGE_IN_PIN_D, EDGE_IN_PIN_CLK},
                 {"Q"}
            ){}

    Pin GetDPin() { return FindPin(EDGE_IN_PIN_D).value(); }
    Pin GetClockPin() { return FindPin(EDGE_IN_PIN_CLK).value(); }
};
