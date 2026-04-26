//
// Created by Gabe on 4/26/2026.
//

#pragma once
#include <memory>


class ConfigManager;
class Module;
class ErrorManager;
class Topbar {
public:
    Topbar() = default;

    static void Render(std::shared_ptr<Module> &module, const std::shared_ptr<ErrorManager> &error_manager);
};
