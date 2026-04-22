//
// Created by gabed on 4/14/2026.
//

#pragma once
#include "Node.h"

class LiteralNode final : public Node {
public:
    [[nodiscard]] std::string type() const override { return "LiteralNode"; }

    void accept(Visitor &v, const int output_slot) override { v.visit(*this, output_slot); }

    [[nodiscard]] nlohmann::json to_json() const override {
        nlohmann::json j = Node::to_json();
        j["value"] = value;
        return j;
    }

    [[nodiscard]] ImVec4 color() const override { return {0.373f, 0.369f, 0.353f, 1.0f}; }

    [[nodiscard]] int width() const override { return 75; }

    void RenderInternals() override;


    LiteralNode(Module *module, const std::string &guid, const int val) : Node(guid, module, "Literal", {}, {"Value"}) {
        value = val;
    }


    int value;
    int bits = 1;
};

inline void LiteralNode::RenderInternals() {
    ImGui::PushItemWidth(60);
    ImGui::SliderInt(("##" + guid).c_str(), &value, 0, powl(2, bits) - 1);
    ImGui::PopItemWidth();
}
