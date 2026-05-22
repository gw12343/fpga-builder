//
// Created by gabed on 4/16/2026.
//

#pragma once

#include "UnaryOpNode.h"


class NotNode final : public UnaryOpNode {
public:
    [[nodiscard]] std::shared_ptr<Node> Clone() const override;
    [[nodiscard]] std::string GetSerializationType() const override { return "NotNode"; }
    [[nodiscard]] std::string GetVerilogAssign(const std::string &out, const std::string &a) const override;

    // Pre-configured node
    NotNode(Module *module, const std::string &guid, int bit_width);
    // New node
    explicit NotNode(Module *parent);

    void accept(Visitor &v, int output_slot) override;
};
