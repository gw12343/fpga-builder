//
// Created by gabed on 4/16/2026.
//

#pragma once

#include "BinaryOpNode.h"


class AndNode final : public BinaryOpNode {
public:
    [[nodiscard]] std::string GetSerializationType() const override { return "AndNode"; }

    [[nodiscard]] std::string GetVerilogAssign(const std::string &out,
                                               const std::vector<std::string> &inputs) const override {
        std::string assign = out + " = " + inputs[0];
        for (int i = 1; i < inputs_size; i++) {
            assign += " & " + inputs[i];
        }
        return assign + ";\n";
    }

    [[nodiscard]] std::shared_ptr<Node> Clone() const override {
        return std::make_unique<AndNode>(module, GUID::generate_guid(), data_bits, inputs_size);
    }

    void accept(Visitor &v, const int output_slot) override { v.visit(*this, output_slot); }


    // Pre-configured node
    AndNode(Module *module, const std::string &guid, const int bit_width, const int num_inputs) :
        BinaryOpNode(module, guid, "AND", bit_width, num_inputs) {}
    // New node
    explicit AndNode(Module *parent) : BinaryOpNode(parent, "AND") {}
};
