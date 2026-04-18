//
// Created by gabed on 4/16/2026.
//

#pragma once

#include "BinaryOpNode.h"


class AndNode : public BinaryOpNode{
public:
    [[nodiscard]] std::string type() const override { return "AndNode"; }
    std::string GetVerilogOperator() const override { return "and"; }
    void accept(Visitor& v) override { v.visit(*this); }


    AndNode(Module *parent, const std::string &guid): BinaryOpNode(parent, guid) { name = "AND Node"; }
};
