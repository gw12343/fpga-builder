//
// Created by gabed on 4/14/2026.
//

#pragma once
#include "GUID.h"
#include "Node.h"


class InputNode final : public Node {
public:
    [[nodiscard]] std::string GetSerializationType() const override { return "InputNode"; }

    [[nodiscard]] nlohmann::json ToJson() const override {
        nlohmann::json j = Node::ToJson();
        j["slot"] = slot;
        return j;
    }

    static constexpr ImVec4 color = {0.094f, 0.373f, 0.647f, 1.0f};
    [[nodiscard]] ImVec4 GetUIColor() const override { return color; }


    void accept(Visitor &v, const int output_slot) override { v.visit(*this, output_slot); }

    [[nodiscard]] std::shared_ptr<Node> Clone() const override {
        return std::make_unique<InputNode>(module, GUID::generate_guid(), slot);
    }

    void RenderInternals() override;
    explicit InputNode(Module *module, const std::string &guid = GUID::generate_guid(), int input = 0);

    void UpdateBits(int bits);

    int slot = 0;
};
