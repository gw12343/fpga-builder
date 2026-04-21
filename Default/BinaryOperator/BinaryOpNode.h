//
// Created by gabed on 4/15/2026.
//

#pragma once
#include "Default/Node.h"

class BinaryOpNode : public Node {
public:
    [[nodiscard]] nlohmann::json to_json() const override {
        nlohmann::json j = Node::to_json();
        return j;
    }

    void accept(Visitor &v, int output_slot) override { v.visit(*this, output_slot); }

    [[nodiscard]] virtual std::string GetVerilogAssign(std::string out, std::string a, std::string b) const {
        return "????";
    }

    BinaryOpNode(Module *parent, const std::string &guid);

    Pin GetAInputPin();
    Pin GetBInputPin();
};
