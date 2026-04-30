//
// Created by Gabe on 4/26/2026.
//

#pragma once
#include <memory>
#include <optional>


class Project;
class OutputViewer;
class ConfigManager;
class ErrorManager;
class Topbar {
public:
    Topbar() = default;

    static void Render(std::shared_ptr<Project> project, const std::shared_ptr<ErrorManager> &error_manager,
                       const std::shared_ptr<OutputViewer> &output_viewer);
};
