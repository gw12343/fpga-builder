//
// Created by gabed on 4/14/2026.
//

#pragma once

#include "ConfigurableTunnelNode.h"


class InputTunnelNode final : public ConfigurableTunnelNode {
public:
    [[nodiscard]] std::shared_ptr<Node> Clone() const override;
    [[nodiscard]] std::string GetSerializationType() const override { return "InputTunnelNode"; }

    [[nodiscard]] ImVec4 GetUIColor() const override { return COLOR; }

    void PushStyle() override;
    void PopStyle() override;

    InputTunnelNode(Module *module, const std::string &guid, std::string net, int bits);
    explicit InputTunnelNode(Module *module);

    void InitPinsAfterConfig() override;


    void accept(Visitor &v, int output_slot) override;

    Pin GetInputPin();

    static constexpr ImVec4 COLOR = {0.0f, 0.0f, 0.0f, 0.0f};
};
