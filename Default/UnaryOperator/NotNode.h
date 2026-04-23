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


    NotNode(Module *parent, const std::string &guid) : UnaryOpNode(parent, guid) { name = "NOT"; }
};
