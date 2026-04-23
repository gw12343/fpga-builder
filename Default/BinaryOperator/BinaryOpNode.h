//
// Created by gabed on 4/15/2026.
//

#pragma once
#include "Default/Node.h"

class BinaryOpNode : public Node {
public:
    [[nodiscard]] nlohmann::json ToJson() const override {
        nlohmann::json j = Node::ToJson();
        return j;
    }

    void accept(Visitor &v, int output_slot) override { v.visit(*this, output_slot); }

    [[nodiscard]] int GetNodeWidth() const override { return 75; }

    [[nodiscard]] ImVec4 GetUIColor() const override { return {0.059f, 0.431f, 0.333f, 1.0f}; }

    [[nodiscard]] virtual std::string GetVerilogAssign(const std::string &out, const std::string &a,
                                                       const std::string &b) const {
        return "????";
    }

    BinaryOpNode(Module *parent, const std::string &guid);

    Pin GetAInputPin();
    Pin GetBInputPin();
};
