//
// Created by gabed on 4/16/2026.
//

#pragma once

#include "BinaryOpNode.h"


class XOrNode final : public BinaryOpNode {
public:
    [[nodiscard]] std::string GetSerializationType() const override { return "XOrNode"; }
    [[nodiscard]] std::string GetVerilogAssign(const std::string &out, const std::string &a,
                                               const std::string &b) const override {
        return out + " = " + a + " ^ " + b + ";\n";
    }
    void accept(Visitor &v, const int output_slot) override { v.visit(*this, output_slot); }

    [[nodiscard]] std::shared_ptr<Node> Clone() const override {
        return std::make_unique<XOrNode>(module, GUID::generate_guid(), bits);
    }

    // Pre-configured node
    XOrNode(Module *module, const std::string &guid, const int bit_width) :
        BinaryOpNode(module, guid, "XOR", bit_width) {}
    // New node
    explicit XOrNode(Module *parent) : BinaryOpNode(parent, "XOR") {}
};
