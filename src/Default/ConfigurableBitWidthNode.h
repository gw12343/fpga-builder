//
// Created by Gabe on 4/25/2026.
//

#pragma once
#include "Node.h"

class ConfigurableBitWidthNode : public Node {
public:
    // Constructor for new node
    ConfigurableBitWidthNode(Module *parent, const std::string &name) : Node(parent, name) { bits = defaultBitsToUse; }
    // Constructor for saved node
    ConfigurableBitWidthNode(const std::string &saved_guid, Module *parent, const std::string &name,
                             const int data_bits) :
        Node(saved_guid, parent, name) {
        bits = data_bits;
    }


    // Save bit width to json
    [[nodiscard]] nlohmann::json ToJson() const override {
        nlohmann::json j = Node::ToJson();
        j["bits"] = bits;
        return j;
    }

    // Override configuration methods
    [[nodiscard]] bool HasConfiguration() const override { return true; }
    void RenderConfiguration() override {
        if (ImGui::DragInt("# Data Bits", &bits, 1, 1, 16)) {
            defaultBitsToUse = bits;
        }
    }

    // Helpers
    [[nodiscard]] int GetDataWidth() const { return bits; }

    static inline int defaultBitsToUse = 4; // TODO save per-project runtime val

protected:
    int bits;
};
