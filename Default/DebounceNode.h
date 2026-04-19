//
// Created by gabed on 4/14/2026.
//

#pragma once
#include "Node.h"


static auto DEBOUNCE_IN_PIN_D =  "D";
static auto DEBOUNCE_IN_PIN_CLK =  "Clk";

class DebounceNode : public Node {
public:

    [[nodiscard]] std::string type() const override { return "DebounceNode"; }

    void accept(Visitor& v) override { v.visit(*this); }


    DebounceNode(Module* module, const std::string &guid) :
            Node(guid, module,
                 "Debounce Node",
                 {DEBOUNCE_IN_PIN_D, DEBOUNCE_IN_PIN_CLK},
                 {"Q"}
            ){}

    Pin GetDPin() { return FindPin(DEBOUNCE_IN_PIN_D).value(); }
    Pin GetClockPin() { return FindPin(DEBOUNCE_IN_PIN_CLK).value(); }
};
