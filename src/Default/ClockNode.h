//
// Created by gabed on 4/14/2026.
//

#pragma once
#include "GUID.h"
#include "Node.h"
#include "UI/Lib/IconsFontAwesome6.h"


class ClockNode final : public Node {
public:
    [[nodiscard]] std::string GetSerializationType() const override { return "ClockNode"; }

    void accept(Visitor &v, const int output_slot) override { v.visit(*this, output_slot); }

    [[nodiscard]] std::shared_ptr<Node> Clone() const override {
        return std::make_unique<ClockNode>(module, GUID::generate_guid());
    }

    [[nodiscard]] int GetNodeWidth() const override { return 25; }
    static constexpr ImVec4 color = {0.1, 0.1, 0.1, .5};
    [[nodiscard]] ImVec4 GetUIColor() const override { return color; }


    explicit ClockNode(Module *module) : ClockNode(module, GUID::generate_guid()) {}

    ClockNode(Module *module, const std::string &guid) :
        Node(guid, module, ICON_FA_WAVE_SQUARE, {}, {{"clk", PinDataType(1)}}) {}
};
