//
// Created by gabed on 4/14/2026.
//

#pragma once
#include "Node.h"


class OutputNode : public Node {

public:

    [[nodiscard]] std::string type() const override { return "OutputNode"; }

    nlohmann::json to_json() const override {
        nlohmann::json j = Node::to_json();
        j["slot"] = slot;
        return j;
    }


    void accept(Visitor& v) override { v.visit(*this); }

    void RenderInternals() override;

    Pin GetValueInputPin();

    OutputNode(Module* module, const std::string &guid, int output);

    int slot = 0;
};
