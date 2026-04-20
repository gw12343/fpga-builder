//
// Created by gabed on 4/15/2026.
//

#pragma once

class EdgeNode;
class DebounceNode;
class DFFNode;
class MultiplexerNode;
class LiteralNode;
class BinaryOpNode;
class OutputNode;
class InputNode;

class Visitor {
public:
    virtual ~Visitor() = default;

    virtual void visit(EdgeNode& node) {}
    virtual void visit(DebounceNode& node) {}
    virtual void visit(DFFNode& node) {}
    virtual void visit(MultiplexerNode& node) {}
    virtual void visit(LiteralNode& node) {}
    virtual void visit(BinaryOpNode& node) {}
    virtual void visit(InputNode& node) {}
    virtual void visit(OutputNode& node) {}
};
