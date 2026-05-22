//
// Created by Gabe on 4/25/2026.
//

#pragma once
#include "Node.h"

class ConfigurableDataAndNumInputsNode : public Node {
public:
    // Constructor for new node
    ConfigurableDataAndNumInputsNode(Module *parent, const std::string &name);
    // Constructor for saved node
    ConfigurableDataAndNumInputsNode(const std::string &saved_guid, Module *parent, const std::string &name,
                                     int data_width, int num_inputs);


    // Save bit widths to json
    [[nodiscard]] nlohmann::json ToJson() const override;

    // Override configuration methods
    [[nodiscard]] bool HasConfiguration() const override { return true; }
    void RenderConfiguration() override;

    // Helpers
    [[nodiscard]] int GetDataWidth() const { return data_bits; }
    [[nodiscard]] int GetNumInputs() const { return inputs_size; }
    [[nodiscard]] std::string GetDisplayName() const override;


    static inline int defaultDataBitsToUse = 4;
    static inline int defaultNumInputs = 2;

protected:
    int data_bits;
    int inputs_size;
};
