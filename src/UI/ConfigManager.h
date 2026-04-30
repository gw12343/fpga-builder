//
// Created by Gabe on 4/26/2026.
//

#pragma once

class Module;
class Node;
class ConfigManager {
public:
    ConfigManager() = default;
    void Render(const std::optional<std::shared_ptr<Module>> &module);
    void ConfigureAndAdd(const std::shared_ptr<Module> &module, const std::shared_ptr<Node> &node);

private:
    std::shared_ptr<Node> current_node;
    bool config_open = false;
};
