//
// Created by Gabe on 4/25/2026.
//

#pragma once

#include "Node.h"

class ConfigurableBitRangeNode : public Node {
public:
    // Constructor for new node
    ConfigurableBitRangeNode(Module *parent, const std::string &name);
    // Constructor for saved node
    ConfigurableBitRangeNode(const std::string &saved_guid, Module *parent, const std::string &name, int width,
                             int start_bit, int end_bit);


    // Save bit widths to json
    [[nodiscard]] nlohmann::json ToJson() const override;

    // Override configuration methods
    [[nodiscard]] bool HasConfiguration() const override { return true; }
    void RenderConfiguration() override;

    // Helpers
    [[nodiscard]] int GetStartBit() const { return range_start_bit; }
    [[nodiscard]] int GetEndBit() const { return range_end_bit; }
    [[nodiscard]] std::string GetDisplayName() const override;
    [[nodiscard]] int GetDataRangeWidth() const;

    static inline int defaultBitWidthToUse = 0;
    static inline int defaultStartBitToUse = 0;
    static inline int defaultEndBitToUse = 3;

protected:
    int input_width;
    int range_start_bit;
    int range_end_bit;
};
