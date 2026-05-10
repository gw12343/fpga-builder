//
// Created by Gabe on 4/25/2026.
//

#pragma once
#include <imgui.h>
#include <nlohmann/json.hpp>
#include <string>


#include "Node.h"

class ConfigurableBitRangeNode : public Node {
public:
    // Constructor for new node
    ConfigurableBitRangeNode(Module *parent, const std::string &name) : Node(parent, name) {
        input_width = defaultBitWidthToUse;
        range_start_bit = defaultStartBitToUse;
        range_end_bit = defaultEndBitToUse;
    }
    // Constructor for saved node
    ConfigurableBitRangeNode(const std::string &saved_guid, Module *parent, const std::string &name, const int width,
                             const int start_bit, const int end_bit) :
        Node(saved_guid, parent, name) {
        input_width = width;
        range_start_bit = start_bit;
        range_end_bit = end_bit;
    }


    // Save bit widths to json
    [[nodiscard]] nlohmann::json ToJson() const override {
        nlohmann::json j = Node::ToJson();
        j["input_width"] = input_width;
        j["range_start_bit"] = range_start_bit;
        j["range_end_bit"] = range_end_bit;
        return j;
    }

    // Override configuration methods
    [[nodiscard]] bool HasConfiguration() const override { return true; }
    void RenderConfiguration() override {
        ImGui::SeparatorText("Input");
        if (ImGui::DragInt("# Bits", &input_width, 1, 1, 16)) {
            defaultBitWidthToUse = input_width;
        }
        ImGui::SeparatorText("Range");
        if (ImGui::DragInt("Start Bit", &range_start_bit, 1, 0, input_width - 1)) {
            defaultStartBitToUse = range_start_bit;
        }
        if (ImGui::DragInt("Range Bit", &range_end_bit, 1, 0, input_width - 1)) {
            defaultEndBitToUse = range_end_bit;
        }
    }

    // Helpers
    [[nodiscard]] int GetStartBit() const { return range_start_bit; }
    [[nodiscard]] int GetEndBit() const { return range_end_bit; }

    [[nodiscard]] std::string GetDisplayName() const override {
        return name + " [" + std::to_string(range_end_bit) + ":" + std::to_string(range_start_bit) + "]";
    }

    int GetDataRangeWidth() const { return labs(range_end_bit - range_start_bit) + 1; }

    static inline int defaultBitWidthToUse = 0;
    static inline int defaultStartBitToUse = 0;
    static inline int defaultEndBitToUse = 3;

protected:
    int input_width;
    int range_start_bit;
    int range_end_bit;
};
