//
// Created by gabed on 4/15/2026.
//

#pragma once

class MultiplexerNode;
class LiteralNode;
class BinaryOpNode;
class OutputNode;
class InputNode;

class Visitor {
public:
    virtual ~Visitor() = default;

    virtual void visit(MultiplexerNode& node) {}
    virtual void visit(LiteralNode& node) {}
    virtual void visit(BinaryOpNode& node) {}
    virtual void visit(InputNode& node) {}
    virtual void visit(OutputNode& node) {}
};
