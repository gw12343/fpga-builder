//
// Created by gabed on 4/15/2026.
//

#pragma once
#include "Visitor.h"

class TraversePrint final : public Visitor {
public:
    void visit(BinaryOpNode &node, int output_slot) override;
    void visit(MultiplexerNode &node, int output_slot) override;
    void visit(LiteralNode &node, int output_slot) override;
    void visit(InputNode &node, int output_slot) override;
    void visit(OutputNode &node, int output_slot) override;
};
