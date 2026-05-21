//
// Created by gabed on 4/16/2026.
//

#pragma once

#include "BinaryOpNode.h"


class NandNode final : public BinaryOpNode {
public:
    [[nodiscard]] std::shared_ptr<Node> Clone() const override;
    [[nodiscard]] std::string GetSerializationType() const override { return "NandNode"; }
    [[nodiscard]] std::string GetVerilogAssign(const std::string &out,
                                               const std::vector<std::string> &inputs) const override;

    void accept(Visitor &v, int output_slot) override;


    // Pre-configured node
    NandNode(Module *module, const std::string &guid, const int bit_width, const int num_inputs) :
        BinaryOpNode(module, guid, "NAND", bit_width, num_inputs) {}
    // New node
    explicit NandNode(Module *parent) : BinaryOpNode(parent, "NAND") {}
};
