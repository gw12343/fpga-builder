//
// Created by gabed on 4/16/2026.
//

#pragma once

#include "BinaryOpNode.h"


class AndNode : public BinaryOpNode{
public:
    [[nodiscard]] std::string type() const override { return "AndNode"; }

    std::string GetVerilogAssign(std::string out, std::string a, std::string b) const { return out + " = " + a + " & " + b + ";\n"; }

    void accept(Visitor& v) override { v.visit(*this); }


    AndNode(Module *parent, const std::string &guid): BinaryOpNode(parent, guid) { name = "AND Node"; }
};
