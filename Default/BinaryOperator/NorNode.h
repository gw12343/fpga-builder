//
// Created by gabed on 4/16/2026.
//

#pragma once

#include "BinaryOpNode.h"


class NorNode : public BinaryOpNode {
public:
    [[nodiscard]] std::string type() const override { return "NorNode"; }
    [[nodiscard]] std::string GetVerilogAssign(std::string out, std::string a, std::string b) const override {
        return out + " = ~(" + a + " | " + b + ");\n";
    }

    void accept(Visitor &v, const int output_slot) override { v.visit(*this, output_slot); }


    NorNode(Module *parent, const std::string &guid) : BinaryOpNode(parent, guid) { name = "NOR Node"; }
};
