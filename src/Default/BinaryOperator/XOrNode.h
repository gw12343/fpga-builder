//
// Created by gabed on 4/16/2026.
//

#pragma once

#include "BinaryOpNode.h"


class XOrNode final : public BinaryOpNode {
public:
    [[nodiscard]] std::string GetSerializationType() const override { return "XOrNode"; }
    [[nodiscard]] std::string GetVerilogAssign(const std::string &out,
                                               const std::vector<std::string> &inputs) const override {
        std::string assign = out + " = " + inputs[0];
        for (int i = 1; i < inputs_size; i++) {
            assign += " ^ " + inputs[i];
        }
        return assign + ";\n";
    }
    void accept(Visitor &v, const int output_slot) override { v.visit(*this, output_slot); }

    [[nodiscard]] std::shared_ptr<Node> Clone() const override {
        return std::make_unique<XOrNode>(module, GUID::generate_guid(), data_bits, inputs_size);
    }

    // Pre-configured node
    XOrNode(Module *module, const std::string &guid, const int bit_width, const int num_inputs) :
        BinaryOpNode(module, guid, "XOR", bit_width, num_inputs) {}
    // New node
    explicit XOrNode(Module *parent) : BinaryOpNode(parent, "XOR") {}
};
