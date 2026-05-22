//
// Created by Gabe on 4/25/2026.
//

#pragma once
#include "Node.h"

class ConfigurableBitWidthNode : public Node {
public:
    // Constructor for new node
    ConfigurableBitWidthNode(Module *parent, const std::string &name);
    // Constructor for saved node
    ConfigurableBitWidthNode(const std::string &saved_guid, Module *parent, const std::string &name, int data_bits);


    // Save bit width to json
    [[nodiscard]] nlohmann::json ToJson() const override;

    // Override configuration methods
    [[nodiscard]] bool HasConfiguration() const override { return true; }
    void RenderConfiguration() override;

    // Helpers
    [[nodiscard]] int GetDataWidth() const { return bits; }
    [[nodiscard]] std::string GetDisplayName() const override;

    static inline int defaultBitsToUse = 4;

protected:
    int bits;
};
