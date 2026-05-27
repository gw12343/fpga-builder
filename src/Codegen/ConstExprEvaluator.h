//
// Created by Gabe on 5/14/2026.
//

#pragma once

#include <memory>
#include <stack>

#include "ResultSpace.h"
#include "UI/ErrorManager.h"


class ConstExprEvaluator : public Visitor {
public:
    explicit ConstExprEvaluator(std::shared_ptr<ErrorManager> error_man);

    ResultSpace FindSpace(const std::shared_ptr<Node> &node, int output_slot);

    ResultSpace EvalNode(const std::optional<Pin> &pin) {
        pin->GetNode().accept(*this, pin->GetNodeIndex());
        const auto _val = m_return_vals.top();
        m_return_vals.pop();
        return _val;
    }

    void visit(OutputTunnelNode &node, int output_slot) override;
    void visit(InputTunnelNode &node, int output_slot) override;
    void visit(BitSelectorNode &node, int output_slot) override;
    void visit(RAMNode &node, int output_slot) override;
    void visit(ShifterNode &node, int output_slot) override;
    void visit(ComparatorNode &node, int output_slot) override;
    void visit(ROMNode &node, int output_slot) override;
    void visit(MultiplierNode &node, int output_slot) override;
    void visit(SubtractorNode &node, int output_slot) override;
    void visit(DecoderNode &node, int output_slot) override;
    void visit(CustomModuleNode &node, int output_slot) override;
    void visit(AdderNode &node, int output_slot) override;
    void visit(RegisterNode &node, int output_slot) override;
    void visit(CombinerNode &node, int output_slot) override;
    void visit(CounterNode &node, int output_slot) override;
    void visit(SplitterNode &node, int output_slot) override;
    void visit(UnaryOpNode &node, int output_slot) override;
    void visit(ClockNode &node, int output_slot) override;
    void visit(EdgeNode &node, int output_slot) override;
    void visit(DebounceNode &node, int output_slot) override;
    void visit(DFFNode &node, int output_slot) override;
    void visit(BinaryOpNode &node, int output_slot) override;
    void visit(MultiplexerNode &node, int output_slot) override;
    void visit(LiteralNode &node, int output_slot) override;
    void visit(InputNode &node, int output_slot) override;
    void visit(OutputNode &node, int output_slot) override;

private:
    bool m_failed = false;
    std::stack<ResultSpace> m_return_vals;
    std::map<std::string, ResultSpace> m_visited_nodes;
    std::shared_ptr<ErrorManager> m_error_manager;
    std::map<std::string, ResultSpace> m_visited_tunnels;

    void CircuitError(const std::string &msg, const Node &node);
};
