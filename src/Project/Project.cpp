//
// Created by Gabe on 4/28/2026.
//

#include "Project.h"

#include <iostream>

#include <filesystem>
#include <fstream>
#include <nlohmann/json.hpp>

#include "CircuitSerializer.h"
#include "Module.h"
#include "UI/Lib/ImGuiNotify.h"


namespace fs = std::filesystem;

#define CONFIG_FILE "/project.fpgabuilder"

Project::Project(const std::string &workspace) : workspace_path(workspace), time_created(0), last_saved(0) {
    if (const fs::path config(workspace + CONFIG_FILE); exists(config)) {
        std::cout << "config exists" << std::endl;

        LoadConfigFile();


        std::cout << "name " << name << std::endl;
        std::cout << "author " << author << std::endl;
        std::cout << "workspace_path " << workspace_path << std::endl;
        std::cout << "time_created " << time_created << std::endl;
        std::cout << "last_saved " << last_saved << std::endl;
    } else {
        // Create new project and save config
        std::cout << "config not exists" << std::endl;

        const auto current_time = std::chrono::duration_cast<std::chrono::milliseconds>(
                                          std::chrono::system_clock::now().time_since_epoch())
                                          .count();

        time_created = current_time;

        name = "New Project";
        author = "gw";

        SaveConfigFile();
    }
}

void Project::Render(const std::shared_ptr<ErrorManager> &error_manager,
                     const std::shared_ptr<CopyPasteManager> &copy_paste_manager) {
    ImGui::Begin("Project Viewer");
    ImGui::Text("Project Name: '%s'", name.c_str());
    ImGui::Text("Author: %s", author.c_str());
    ImGui::SeparatorText("MODULES");

    int i = 0;
    for (const auto &module: modules) {
        const bool is_top_level = top_level_node_guid == module->guid;

        if (std::string prefix = is_top_level ? ICON_FA_STAR : "";
            ImGui::Button((prefix + module->name).c_str(), ImVec2(ImGui::GetContentRegionAvail().x, 0.0f))) {
            selected_module = i;
        }
        i++;
    }

    const auto &sel = GetSelectedModule();


    ImGuiWindowClass window_class;
    window_class.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoTabBar;
    ImGui::SetNextWindowClass(&window_class);

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("Node Editor Win", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoTitleBar);


    if (sel.has_value()) {
        const auto &selected_module = sel->get();
        selected_module->Render(error_manager, copy_paste_manager);
    } else {
        ImGui::Text("Select a module to get started.");
    }

    // render


    ImGui::End();
    ImGui::PopStyleVar();


    ImGui::End();
}

void Project::RegisterModule(const std::shared_ptr<Module> &m) {
    modules.push_back(m);
    for (const auto &module: modules) {
        module->RefreshAllCustomModuleNodes(m);
    }

    if (m->guid == top_level_node_guid)
        selected_module = modules.size() - 1;
}

std::optional<std::shared_ptr<Module>> Project::GetSelectedModule() {
    if (selected_module >= 0 && selected_module < modules.size()) {
        return modules[selected_module];
    }
    return std::nullopt;
}
std::optional<std::shared_ptr<Module>> Project::GetModule(const std::string &guid) const {
    for (const auto &module: modules) {
        if (module->guid == guid) {
            return module;
        }
    }
    return std::nullopt;
}

void Project::SaveConfigFile() {
    nlohmann::json j_file;

    j_file["name"] = name;
    j_file["author"] = author;
    j_file["workspace_path"] = workspace_path;

    j_file["selected_module"] = selected_module;
    j_file["top_level_node_guid"] = top_level_node_guid;
    j_file["time_created"] = time_created;
    j_file["last_saved"] =
            std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch())
                    .count();


    const auto &config_path = workspace_path + CONFIG_FILE;

    if (std::ofstream file(config_path); file.is_open()) {
        std::cout << "Writing output file..." << std::endl;
        file << j_file.dump(4);
        file.close();

        ImGui::InsertNotification({ImGuiToastType::Success, 3000, "Saved config to %s", config_path.c_str()});

    } else {
        std::cerr << "Could not open file \"" << config_path << "\"" << std::endl;
        ImGui::InsertNotification({ImGuiToastType::Error, 3000, "Failed to save project config '%s' to %s",
                                   name.c_str(), config_path.c_str()});
    }
}


void Project::LoadConfigFile() {
    const auto &config_path = workspace_path + CONFIG_FILE;
    std::cout << "loading project " << config_path << std::endl;


    // Open and read config file
    std::ifstream file(config_path);
    std::stringstream buffer;
    buffer << file.rdbuf();

    std::cout << buffer.str() << std::endl;

    // Parse data
    nlohmann::json j = nlohmann::json::parse(buffer.str());

    // Create Module

    name = j["name"].get<std::string>();
    author = j["author"].get<std::string>();
    workspace_path = j["workspace_path"].get<std::string>();
    top_level_node_guid = j["top_level_node_guid"].get<std::string>();

    time_created = j["time_created"].get<long long>();
    last_saved = j["last_saved"].get<long long>();

    selected_module = j["selected_module"].get<int>();


    auto module_files = j["module_files"].get<std::vector<std::string>>();

    for (const auto &path: module_files) {
        auto m = CircuitSerializer::LoadModule(this, workspace_path + "/" + path);
        RegisterModule(m);
    }

    ImGui::InsertNotification({ImGuiToastType::Success, 3000, "Loaded config for project '%s'", name.c_str()});
}
