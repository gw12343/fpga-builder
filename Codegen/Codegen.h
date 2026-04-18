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

     void visit(BinaryOpNode& node) override;
     void visit(MultiplexerNode& node) override;
     void visit(LiteralNode& node) override;
     void visit(InputNode& node) override;
     void visit(OutputNode& node) override;

private:
     std::string GetSafeWireName(const std::string& wire_name);
     static void CircuitError(const std::string& msg, const Node& node);
     std::string code;
     std::stack<std::string> returnVals;

     std::map<std::string, int> wireNameCounts;
};
