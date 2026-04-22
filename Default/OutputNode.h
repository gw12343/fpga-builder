//
// Created by gabed on 4/14/2026.
//

#pragma once
#include "Node.h"


class OutputNode : public Node {

public:
    [[nodiscard]] std::string type() const override { return "OutputNode"; }

    [[nodiscard]] nlohmann::json to_json() const override {
        nlohmann::json j = Node::to_json();
        j["slot"] = slot;
        return j;
    }

    [[nodiscard]] ImVec4 color() const override { return {0.094f, 0.373f, 0.647f, 1.0f}; }


    void accept(Visitor &v, const int output_slot) override { v.visit(*this, output_slot); }

    void RenderInternals() override;

    Pin GetValueInputPin();

    OutputNode(Module *module, const std::string &guid, int output);

    int slot = 0;
};
