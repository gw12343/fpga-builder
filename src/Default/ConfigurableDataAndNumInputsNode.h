//
// Created by Gabe on 4/25/2026.
//

#pragma once
#include <imgui.h>
#include <nlohmann/json.hpp>


#include "Node.h"

class ConfigurableDataAndNumInputsNode : public Node {
public:
    // Constructor for new node
    ConfigurableDataAndNumInputsNode(Module *parent, const std::string &name) : Node(parent, name) {
        data_bits = defaultDataBitsToUse;
        inputs_size = defaultNumInputs;
    }
    // Constructor for saved node
    ConfigurableDataAndNumInputsNode(const std::string &saved_guid, Module *parent, const std::string &name,
                                     const int data_width, const int num_inputs) :
        Node(saved_guid, parent, name) {
        data_bits = data_width;
        inputs_size = num_inputs;
    }


    // Save bit widths to json
    [[nodiscard]] nlohmann::json ToJson() const override {
        nlohmann::json j = Node::ToJson();
        j["bits"] = data_bits;
        j["num_inputs"] = inputs_size;
        return j;
    }

    // Override configuration methods
    [[nodiscard]] bool HasConfiguration() const override { return true; }
    void RenderConfiguration() override {
        if (ImGui::DragInt("# Data Bits", &data_bits, 1, 1, 16)) {
            defaultDataBitsToUse = data_bits;
        }
        if (ImGui::DragInt("# Inputs", &inputs_size, 1, 2, 10)) {
            defaultNumInputs = inputs_size;
        }
    }

    // Helpers
    [[nodiscard]] int GetDataWidth() const { return data_bits; }
    [[nodiscard]] int GetNumInputs() const { return inputs_size; }
    [[nodiscard]] std::string GetDisplayName() const override { return name + " (" + std::to_string(data_bits) + ")"; }


    static inline int defaultDataBitsToUse = 4;
    static inline int defaultNumInputs = 2;

protected:
    int data_bits;
    int inputs_size;
};
