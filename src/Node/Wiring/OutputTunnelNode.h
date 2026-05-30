//
// Created by gabed on 4/14/2026.
//

#pragma once

#include "ConfigurableTunnelNode.h"


class OutputTunnelNode final : public ConfigurableTunnelNode {
public:
    [[nodiscard]] std::shared_ptr<Node> Clone() const override;
    [[nodiscard]] std::string GetSerializationType() const override { return "OutputTunnelNode"; }

    [[nodiscard]] ImVec4 GetUIColor() const override { return COLOR; }

    void PushStyle() override;
    void PopStyle() override;

    OutputTunnelNode(Module *module, const std::string &guid, std::string net, int bits);
    explicit OutputTunnelNode(Module *module);

    void InitPinsAfterConfig() override;


    void accept(Visitor &v, int output_slot) override;

    static constexpr ImVec4 COLOR = {0.0f, 0.0f, 0.0f, 0.0f};
};
