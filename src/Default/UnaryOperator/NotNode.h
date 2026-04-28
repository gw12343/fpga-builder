//
// Created by gabed on 4/16/2026.
//

#pragma once

#include "UnaryOpNode.h"


class NotNode final : public UnaryOpNode {
public:
    [[nodiscard]] std::string GetSerializationType() const override { return "NotNode"; }

    [[nodiscard]] std::string GetVerilogAssign(const std::string &out, const std::string &a) const override {
        return out + " = ~" + a + ";\n";
    }

    void accept(Visitor &v, const int output_slot) override { v.visit(*this, output_slot); }

    [[nodiscard]] std::shared_ptr<Node> Clone() const override {
        return std::make_unique<NotNode>(module, GUID::generate_guid(), bits);
    }

    // Pre-configured node
    NotNode(Module *module, const std::string &guid, const int bit_width) :
        UnaryOpNode(module, guid, "NOT", bit_width) {}
    // New node
    explicit NotNode(Module *parent) : UnaryOpNode(parent, "NOT") {}
};
