//
// Created by gabed on 4/15/2026.
//

#pragma once
#include <map>
#include <stack>

#include "Visitor.h"
#include "Module.h"

class Codegen final : public Visitor {
public:

     void GenerateCode(const std::shared_ptr<Module>& module);

     void visit(DebounceNode& node) override;
     void visit(DFFNode& node) override;
     void visit(BinaryOpNode& node) override;
     void visit(MultiplexerNode& node) override;
     void visit(LiteralNode& node) override;
     void visit(InputNode& node) override;
     void visit(OutputNode& node) override;

private:
     std::optional<std::string> CheckCache(const std::string& guid);
     bool CheckActive(const std::string& guid);
     std::string GetSafeWireName(const std::string& wire_name);
     static void CircuitError(const std::string& msg, const Node& node);

    template<typename N, typename = std::enable_if_t<std::is_base_of_v<Node, N>>>
    std::string EvalNode(N &node) {
        node.accept(*this);
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
};
