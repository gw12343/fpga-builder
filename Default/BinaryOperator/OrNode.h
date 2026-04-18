//
// Created by gabed on 4/16/2026.
//

#pragma once

#include "BinaryOpNode.h"


class OrNode : public BinaryOpNode{
public:
    [[nodiscard]] std::string type() const override { return "OrNode"; }
    std::string GetVerilogOperator() const override { return "or"; }
    void accept(Visitor& v) override { v.visit(*this); }


    OrNode(Module *parent, const std::string &guid): BinaryOpNode(parent, guid) { name = "OR Node"; }
};
