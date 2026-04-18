//
// Created by gabed on 4/15/2026.
//

#pragma once
#include "Visitor.h"

class TraversePrint final : public Visitor{
public:
     void visit(BinaryOpNode& node) override;
     void visit(MultiplexerNode& node) override;
     void visit(LiteralNode& node) override;
     void visit(InputNode& node) override;
     void visit(OutputNode& node) override;
};
