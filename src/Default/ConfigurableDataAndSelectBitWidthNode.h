//
// Created by Gabe on 4/25/2026.
//

#pragma once
#include "Node.h"

class ConfigurableDataAndSelectBitWidthNode : public Node {
public:
    // Constructor for new node
    ConfigurableDataAndSelectBitWidthNode(Module *parent, const std::string &name) : Node(parent, name) {
        data_bits = defaultDataBitsToUse;
        select_bits = defaultSelectBitsToUse;
    }
    // Constructor for saved node
    ConfigurableDataAndSelectBitWidthNode(const std::string &saved_guid, Module *parent, const std::string &name,
                                          const int data_width, const int select_width) :
        Node(saved_guid, parent, name) {
        data_bits = data_width;
        select_bits = select_width;
    }


    // Save bit width to json
    [[nodiscard]] nlohmann::json ToJson() const override {
        nlohmann::json j = Node::ToJson();
        j["data_bits"] = data_bits;
        j["select_bits"] = select_bits;
        return j;
    }

    // Override configuration methods
    [[nodiscard]] bool HasConfiguration() const override { return true; }
    void RenderConfiguration() override {
        if (ImGui::DragInt("# Data Bits", &data_bits, 1, 1, 16)) {
            defaultDataBitsToUse = data_bits;
        }
        if (ImGui::DragInt("# Select Bits", &select_bits, 1, 1, 4)) {
            defaultSelectBitsToUse = select_bits;
        }
    }

    // Helpers
    [[nodiscard]] int GetDataWidth() const { return data_bits; }
    [[nodiscard]] int GetSelectWidth() const { return select_bits; }

    static inline int defaultDataBitsToUse = 4;
    static inline int defaultSelectBitsToUse = 1;

protected:
    int data_bits;
    int select_bits;
};
