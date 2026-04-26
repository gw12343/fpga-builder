//
// Created by gabed on 4/16/2026.
//

#pragma once

#include "BinaryOpNode.h"


class NorNode final : public BinaryOpNode {
public:
    [[nodiscard]] std::string GetSerializationType() const override { return "NorNode"; }
    [[nodiscard]] std::string GetVerilogAssign(const std::string &out, const std::string &a,
                                               const std::string &b) const override {
        return out + " = ~(" + a + " | " + b + ");\n";
    }

    void accept(Visitor &v, const int output_slot) override { v.visit(*this, output_slot); }


    // Pre-configured node
    NorNode(Module *module, const std::string &guid, const int bit_width) :
        BinaryOpNode(module, guid, "NOR", bit_width) {}
    // New node
    explicit NorNode(Module *parent) : BinaryOpNode(parent, "NOR") {}
};
