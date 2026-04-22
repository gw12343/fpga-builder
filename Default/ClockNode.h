//
// Created by gabed on 4/14/2026.
//

#pragma once
#include "IconsFontAwesome6.h"
#include "Node.h"


class ClockNode : public Node {
public:
    [[nodiscard]] std::string type() const override { return "ClockNode"; }

    void accept(Visitor &v, const int output_slot) override { v.visit(*this, output_slot); }

    [[nodiscard]] int width() const override { return 25; }
    [[nodiscard]] ImVec4 color() const override { return {0, 0, 0, 0}; }

    ClockNode(Module *module, const std::string &guid) : Node(guid, module, ICON_FA_WAVE_SQUARE, {}, {"clk"}) {}
};
