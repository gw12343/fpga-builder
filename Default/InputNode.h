//
// Created by gabed on 4/14/2026.
//

#pragma once
#include "Node.h"


class InputNode final : public Node {
public:
    [[nodiscard]] std::string GetSerializationType() const override { return "InputNode"; }

    [[nodiscard]] nlohmann::json ToJson() const override {
        nlohmann::json j = Node::ToJson();
        j["slot"] = slot;
        return j;
    }

    [[nodiscard]] ImVec4 GetUIColor() const override { return {0.094f, 0.373f, 0.647f, 1.0f}; }


    void accept(Visitor &v, const int output_slot) override { v.visit(*this, output_slot); }

    void RenderInternals() override;
    InputNode(Module *module, const std::string &guid, int input);

    int slot = 0;
};
