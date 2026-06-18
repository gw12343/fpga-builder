//
// Created by gabed on 4/14/2026.
//

#pragma once

#include "Node/Node.h"


class ClockNode final : public Node {
public:
    [[nodiscard]] std::shared_ptr<Node> Clone() const override;
    [[nodiscard]] std::string GetSerializationType() const override { return "ClockNode"; }
    [[nodiscard]] int GetNodeWidth() const override { return 25; }
    [[nodiscard]] ImVec4 GetUIColor() const override { return COLOR; }
    [[nodiscard]] bool IsSequential() const override { return false; }

    explicit ClockNode(Module *module);
    ClockNode(Module *module, const std::string &guid);


    void accept(Visitor &v, int output_slot) override;

    static constexpr ImVec4 COLOR = {0.1, 0.1, 0.1, .5};
};
