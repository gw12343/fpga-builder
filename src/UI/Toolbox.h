//
// Created by Gabe on 4/26/2026.
//

#pragma once
#include <memory>

#include "ConfigManager.h"
#include "Default/Node.h"
#include "Module.h"

using NodeCreator = std::function<std::shared_ptr<Node>(const std::shared_ptr<Module> &)>;

struct NodeToolboxData {
    std::string name;
    ImVec4 color;
    NodeCreator creator;
};

class Toolbox {
public:
    Toolbox();
    void Render(std::optional<std::shared_ptr<Module>> module, const std::shared_ptr<ConfigManager> &config_manager);

private:
    std::map<std::string, std::vector<NodeToolboxData>> categories;
};
