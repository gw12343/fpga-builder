//
// Created by gabed on 4/14/2026.
//

#pragma once
#include "Node.h"


static auto IN_PIN_SET =  "Set";
static auto IN_PIN_RESET =  "Reset";
static auto IN_PIN_D =  "D";
static auto IN_PIN_CLOCK =  "Clk";

class DFFNode : public Node {
public:

    [[nodiscard]] std::string type() const override { return "DFFNode"; }

    void accept(Visitor& v) override { v.visit(*this); }


    DFFNode(Module* module, const std::string &guid) :
            Node(guid, module,
                 "D Flip Flop Node",
                 {IN_PIN_SET, IN_PIN_RESET, IN_PIN_D, IN_PIN_CLOCK},
                 {"Value"}
            ){}

    Pin GetSetPin() { return FindPin(IN_PIN_SET).value(); }
    Pin GetResetPin() { return FindPin(IN_PIN_RESET).value(); }
    Pin GetDPin() { return FindPin(IN_PIN_D).value(); }
    Pin GetClkPin() { return FindPin(IN_PIN_CLOCK).value(); }
};
