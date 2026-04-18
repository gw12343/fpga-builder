//
// Created by gabed on 4/15/2026.
//

#pragma once
#include "Default/Node.h"

class BinaryOpNode : public Node {
public:

    nlohmann::json to_json() const override {
        nlohmann::json j = Node::to_json();
        return j;
    }

    void accept(Visitor& v) override { v.visit(*this); }

    virtual std::string GetVerilogOperator() const { return "????"; }

    BinaryOpNode(Module* parent, const std::string &guid);

    Pin GetAInputPin();
    Pin GetBInputPin();

};
