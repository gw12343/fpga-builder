//
// Created by gabed on 4/14/2026.
//

#pragma once
#include "Node.h"


class InputNode : public Node {
public:
    [[nodiscard]] std::string type() const override { return "InputNode"; }

    [[nodiscard]] nlohmann::json to_json() const override {
        nlohmann::json j = Node::to_json();
        j["slot"] = slot;
        return j;
    }


    void accept(Visitor &v, const int output_slot) override { v.visit(*this, output_slot); }

    void RenderInternals() override;
    InputNode(Module *module, const std::string &guid, int input);

    int slot = 0;
};
