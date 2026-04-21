//
// Created by gabed on 4/16/2026.
//

#pragma once

#include "BinaryOpNode.h"


class XOrNode : public BinaryOpNode {
public:
    [[nodiscard]] std::string type() const override { return "XOrNode"; }
    [[nodiscard]] std::string GetVerilogAssign(std::string out, std::string a, std::string b) const override {
        return out + " = " + a + " ^ " + b + ";\n";
    }
    void accept(Visitor &v, const int output_slot) override { v.visit(*this, output_slot); }


    XOrNode(Module *parent, const std::string &guid) : BinaryOpNode(parent, guid) { name = "XOR Node"; }
};
