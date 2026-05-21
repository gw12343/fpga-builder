//
// Created by gabed on 4/16/2026.
//

#pragma once

#include "BinaryOpNode.h"


class OrNode final : public BinaryOpNode {
public:
    [[nodiscard]] std::shared_ptr<Node> Clone() const override;
    [[nodiscard]] std::string GetSerializationType() const override { return "OrNode"; }
    [[nodiscard]] std::string GetVerilogAssign(const std::string &out,
                                               const std::vector<std::string> &inputs) const override;

    void accept(Visitor &v, int output_slot) override;

    // Pre-configured node
    OrNode(Module *module, const std::string &guid, int bit_width, int num_inputs);
    // New node
    explicit OrNode(Module *parent);
};
