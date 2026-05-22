//
// Created by gabed on 4/14/2026.
//

#pragma once

#include "GUID.h"
#include "Node/Node.h"


class InputNode final : public Node {
public:
    [[nodiscard]] std::shared_ptr<Node> Clone() const override;
    [[nodiscard]] std::string GetSerializationType() const override { return "InputNode"; }
    [[nodiscard]] nlohmann::json ToJson() const override;
    [[nodiscard]] ImVec4 GetUIColor() const override { return COLOR; }

    explicit InputNode(Module *module, const std::string &guid = GUID::generate_guid(), int input = 0);

    void RenderInternals() override;
    void UpdateBits(int bits);
    void accept(Visitor &v, int output_slot) override;

    static constexpr ImVec4 COLOR = {0.094f, 0.373f, 0.647f, 1.0f};
    int slot = 0;
};
