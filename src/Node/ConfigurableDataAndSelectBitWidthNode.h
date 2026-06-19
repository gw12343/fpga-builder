//
// Created by Gabe on 4/25/2026.
//

#pragma once
#include "Node.h"

class ConfigurableDataAndSelectBitWidthNode : public Node {
public:
    // Constructor for new node
    ConfigurableDataAndSelectBitWidthNode(Module *parent, const std::string &name);
    // Constructor for saved node
    ConfigurableDataAndSelectBitWidthNode(const std::string &saved_guid, Module *parent, const std::string &name,
                                          int data_width, int select_width);


    // Save bit widths to json
    [[nodiscard]] nlohmann::json ToJson() const override;

    // Override configuration methods
    [[nodiscard]] bool HasConfiguration() const override { return true; }
    void RenderConfiguration() override;

    // Helpers
    [[nodiscard]] int GetDataWidth() const { return m_data_bits; }
    [[nodiscard]] int GetSelectWidth() const { return m_select_bits; }
    [[nodiscard]] std::string GetDisplayName() const override;


    static inline int defaultDataBitsToUse = 4;
    static inline int defaultSelectBitsToUse = 1;

protected:
    int m_data_bits;
    int m_select_bits;
};
