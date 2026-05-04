//
// Created by gabed on 4/15/2026.
//

#pragma once

#include <stack>


#include "Module.h"
#include "UI/ErrorManager.h"
#include "Visitor.h"

class Codegen final : public Visitor {
public:
    explicit Codegen(std::shared_ptr<ErrorManager> error_man);

    void GenerateCode(const std::shared_ptr<Module> &module);

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
    std::optional<std::string> CheckCache(const std::string &guid);
    bool CheckActive(const std::string &guid);
    std::string GetSafeWireName(const std::string &wire_name);
    void CircuitError(const std::string &msg, const Node &node);

    std::string EvalNode(const std::optional<Pin> &pin) {
        pin->GetNode().accept(*this, pin->GetNodeIndex());
        auto _val = m_return_vals.top();
        m_return_vals.pop();
        return _val;
    }


    std::string m_decls;
    std::string m_instances;
    std::string m_inner;
    std::string m_later;
    bool m_failed;


    std::stack<std::string> m_return_vals;
    std::stack<std::string> m_active_nodes;

    std::map<std::string, int> m_wire_name_counts;
    std::map<std::string, std::string> m_visited_nodes;

    std::shared_ptr<ErrorManager> m_error_manager;
};
