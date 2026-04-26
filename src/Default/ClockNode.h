//
// Created by gabed on 4/14/2026.
//

#pragma once
#include "GUID.h"
#include "IconsFontAwesome6.h"
#include "Node.h"


class ClockNode final : public Node {
public:
    [[nodiscard]] std::string GetSerializationType() const override { return "ClockNode"; }

    void accept(Visitor &v, const int output_slot) override { v.visit(*this, output_slot); }

    [[nodiscard]] int GetNodeWidth() const override { return 25; }
    [[nodiscard]] ImVec4 GetUIColor() const override { return {0, 0, 0, 0}; }


    explicit ClockNode(Module *module) : ClockNode(module, GUID::generate_guid()) {}

    ClockNode(Module *module, const std::string &guid) :
        Node(guid, module, ICON_FA_WAVE_SQUARE, {}, {{"clk", PinDataType(1)}}) {}
};
