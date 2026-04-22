//
// Created by gabed on 4/16/2026.
//

#pragma once

#include "BinaryOpNode.h"


class OrNode final : public BinaryOpNode {
public:
    [[nodiscard]] std::string type() const override { return "OrNode"; }
    [[nodiscard]] std::string GetVerilogAssign(const std::string &out, const std::string &a,
                                               const std::string &b) const override {
        return out + " = " + a + " | " + b + ";\n";
    }
    void accept(Visitor &v, const int output_slot) override { v.visit(*this, output_slot); }


    OrNode(Module *parent, const std::string &guid) : BinaryOpNode(parent, guid) { name = "OR"; }
};
