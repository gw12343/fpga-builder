//
// Created by gabed on 4/16/2026.
//

#pragma once

#include "BinaryOpNode.h"


class NorNode final : public BinaryOpNode {
public:
    [[nodiscard]] std::shared_ptr<Node> Clone() const override;
    [[nodiscard]] std::string GetSerializationType() const override { return "NorNode"; }
    [[nodiscard]] std::string GetVerilogAssign(const std::string &out,
                                               const std::vector<std::string> &inputs) const override;

    void accept(Visitor &v, int output_slot) override;


    // Pre-configured node
    NorNode(Module *module, const std::string &guid, const int bit_width, const int num_inputs) :
        BinaryOpNode(module, guid, "NOR", bit_width, num_inputs) {}
    // New node
    explicit NorNode(Module *parent) : BinaryOpNode(parent, "NOR") {}
};
