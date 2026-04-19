//
// Created by gabed on 4/16/2026.
//

#pragma once

#include "BinaryOpNode.h"


class NorNode : public BinaryOpNode{
public:
    [[nodiscard]] std::string type() const override { return "NorNode"; }
    std::string GetVerilogAssign(std::string out, std::string a, std::string b) const { return out + " = ~(" + a + " | " + b + ");\n"; }

    void accept(Visitor& v) override { v.visit(*this); }


    NorNode(Module *parent, const std::string &guid): BinaryOpNode(parent, guid) { name = "NOR Node"; }
};
