//
// Created by Gabe on 5/26/2026.
//

#pragma once
#include "Node/Node.h"

class ConfigurableTunnelNode : public Node {
public:
    // Constructor for new node
    ConfigurableTunnelNode(Module *parent, const std::string &name);
    // Constructor for saved node
    ConfigurableTunnelNode(const std::string &saved_guid, Module *parent, const std::string &name, int data_width,
                           const std::string &net);

    // Save bit width and tunnel name to json
    [[nodiscard]] nlohmann::json ToJson() const override;

    // Override configuration methods
    [[nodiscard]] bool HasConfiguration() const override { return true; }
    void RenderConfiguration() override;

    // Helpers
    [[nodiscard]] int GetDataWidth() const { return data_bits; }
    [[nodiscard]] const std::string &GetNetName() const { return net_name; }
    [[nodiscard]] std::string GetDisplayName() const override;

    [[nodiscard]] std::string GetTunnelPinName() const;
    [[nodiscard]] int GetNodeWidth() const override;


    static inline int defaultDataBitsToUse = 4;
    static inline std::string defaultNetNameToUse = "";

protected:
    int data_bits;
    std::string net_name;
};
