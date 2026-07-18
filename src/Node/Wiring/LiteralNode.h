//
// Created by gabed on 4/14/2026.
//

#pragma once

#include "Node/ConfigurableBitWidthNode.h"

class LiteralNode final : public ConfigurableBitWidthNode {
public:
    [[nodiscard]] std::string GetSerializationType() const override { return "LiteralNode"; }
    [[nodiscard]] std::shared_ptr<Node> Clone() const override;
    [[nodiscard]] nlohmann::json ToJson() const override;
    [[nodiscard]] ImVec4 GetUIColor() const override { return COLOR; }
    [[nodiscard]] int GetNodeWidth() const override { return 125; }
    [[nodiscard]] bool IsSequential() const override { return false; }

    // Pre-configured
    LiteralNode(Module *module, const std::string &guid, int bit_width, uint64_t val);
    // New node
    explicit LiteralNode(Module *module);


    void RenderInternals() override;
    void InitPinsAfterConfig() override;
    void accept(Visitor &v, int output_slot) override;


    static constexpr ImVec4 COLOR = {0.373f, 0.369f, 0.353f, 1.0f};
    uint64_t value;
};
