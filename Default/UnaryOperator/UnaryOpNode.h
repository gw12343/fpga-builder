//
// Created by gabed on 4/15/2026.
//

#pragma once
#include "Default/Node.h"

class UnaryOpNode : public Node {
public:
    [[nodiscard]] nlohmann::json ToJson() const override {
        nlohmann::json j = Node::ToJson();
        return j;
    }

    void accept(Visitor &v, int output_slot) override { v.visit(*this, output_slot); }

    [[nodiscard]] int GetNodeWidth() const override { return 75; }
    [[nodiscard]] ImVec4 GetUIColor() const override { return {0.114f, 0.616f, 0.647f, 1.0f}; }


    [[nodiscard]] virtual std::string GetVerilogAssign(const std::string &out, const std::string &a) const {
        return "????";
    }

    UnaryOpNode(Module *parent, const std::string &guid);

    Pin GetAInputPin();
};
