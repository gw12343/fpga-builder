//
// Created by gabed on 4/15/2026.
//

#pragma once


class CounterNode;
class SplitterNode;
class UnaryOpNode;
class ClockNode;
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

    virtual void visit(CounterNode &node, int output_slot) {}
    virtual void visit(SplitterNode &node, int output_slot) {}
    virtual void visit(UnaryOpNode &node, int output_slot) {}
    virtual void visit(ClockNode &node, int output_slot) {}
    virtual void visit(EdgeNode &node, int output_slot) {}
    virtual void visit(DebounceNode &node, int output_slot) {}
    virtual void visit(DFFNode &node, int output_slot) {}
    virtual void visit(MultiplexerNode &node, int output_slot) {}
    virtual void visit(LiteralNode &node, int output_slot) {}
    virtual void visit(BinaryOpNode &node, int output_slot) {}
    virtual void visit(InputNode &node, int output_slot) {}
    virtual void visit(OutputNode &node, int output_slot) {}
};
