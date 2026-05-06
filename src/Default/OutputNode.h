//
// Created by gabed on 4/14/2026.
//

#pragma once
#include "GUID.h"
#include "Node.h"


class OutputNode final : public Node {

public:
    [[nodiscard]] std::string GetSerializationType() const override { return "OutputNode"; }

    [[nodiscard]] nlohmann::json ToJson() const override {
        nlohmann::json j = Node::ToJson();
        j["slot"] = slot;
        return j;
    }

    static constexpr ImVec4 COLOR = {0.094f, 0.373f, 0.647f, 1.0f};
    [[nodiscard]] ImVec4 GetUIColor() const override { return COLOR; }


    void accept(Visitor &v, const int output_slot) override { v.visit(*this, output_slot); }

    [[nodiscard]] std::shared_ptr<Node> Clone() const override {
        return std::make_unique<OutputNode>(module, GUID::generate_guid(), slot);
    }

    void RenderInternals() override;

    Pin GetValueInputPin();

    explicit OutputNode(Module *module, const std::string &guid = GUID::generate_guid(), int output = 0);

    void UpdateBits(int bits);

    int slot = 0;
};
