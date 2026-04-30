//
// Created by gabed on 4/16/2026.
//

#pragma once

#include "BinaryOpNode.h"
#include "GUID.h"

class OrNode final : public BinaryOpNode {
public:
    [[nodiscard]] std::string GetSerializationType() const override { return "OrNode"; }
    [[nodiscard]] std::string GetVerilogAssign(const std::string &out, const std::string &a,
                                               const std::string &b) const override {
        return out + " = " + a + " | " + b + ";\n";
    }
    void accept(Visitor &v, const int output_slot) override { v.visit(*this, output_slot); }

    [[nodiscard]] std::shared_ptr<Node> Clone() const override {
        return std::make_unique<OrNode>(module, GUID::generate_guid(), bits);
    }

    // Pre-configured node
    OrNode(Module *module, const std::string &guid, const int bit_width) :
        BinaryOpNode(module, guid, "OR", bit_width) {}
    // New node
    explicit OrNode(Module *parent) : BinaryOpNode(parent, "OR") {}
};
