//
// Created by Gabe on 4/26/2026.
//

#pragma once
#include <memory>

#include "ConfigManager.h"
#include "Module.h"


class Toolbox {
public:
    Toolbox() = default;

    static void Render(const std::shared_ptr<Module> &module, const std::shared_ptr<ConfigManager> &config_manager);
};
