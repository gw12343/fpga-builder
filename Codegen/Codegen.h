//
// Created by gabed on 4/15/2026.
//

#pragma once
#include <iostream>
#include <map>
#include <stack>

#include "ErrorManager.h"
#include "Module.h"
#include "Visitor.h"

class Codegen final : public Visitor {
public:
    explicit Codegen(std::shared_ptr<ErrorManager> error_man);

    void GenerateCode(const std::shared_ptr<Module> &module);

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
    void CircuitError(const std::string &msg, const Node &node) const;

    std::string EvalNode(const std::optional<Pin> &pin) {
        pin->GetNode().accept(*this, pin->GetNodeIndex());
        auto _val = returnVals.top();
        returnVals.pop();
        return _val;
    }


    std::string decls;
    std::string inner;
    std::string later;


    std::stack<std::string> returnVals;
    std::stack<std::string> activeNodes;

    std::map<std::string, int> wireNameCounts;
    std::map<std::string, std::string> visitedNodes;

    std::shared_ptr<ErrorManager> error_manager;
};
