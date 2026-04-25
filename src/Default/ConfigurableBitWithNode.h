//
// Created by Gabe on 4/25/2026.
//

#pragma once
#include "Node.h"

class ConfigurableBitWidthNode : public Node {
public:
    // Pass data to same headers
    ConfigurableBitWidthNode(Module *parent, const std::string &name, const std::vector<PinCreationData> &inputs,
                             const std::vector<PinCreationData> &outputs, const int bit_width) :
        Node(parent, name, inputs, outputs), bits(bit_width) {}
    ConfigurableBitWidthNode(const std::string &saved_guid, Module *parent, const std::string &name,
                             const std::vector<PinCreationData> &inputs, const std::vector<PinCreationData> &outputs,
                             const int bit_width) :
        Node(saved_guid, parent, name, inputs, outputs), bits(bit_width) {}

    // Save bit width to json
    [[nodiscard]] nlohmann::json ToJson() const override {
        nlohmann::json j = Node::ToJson();
        j["bits"] = bits;
        return j;
    }

    // Override configuration methods
    [[nodiscard]] bool HasConfiguration() const override { return true; }

private:
    int bits;
};
