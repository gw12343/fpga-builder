//
// Created by Gabe on 4/28/2026.
//

#include "Project.h"

#include <iostream>

#include <filesystem>
#include <fstream>


#include "CircuitSerializer.h"
#include "Module.h"


namespace fs = std::filesystem;

#define CONFIG_FILE "/project.fpgabuilder"

Project::Project(const std::string &workspace) : m_time_created(0), m_last_saved(0), m_workspace_path(workspace) {
    if (const fs::path config(workspace + CONFIG_FILE); exists(config)) {
        std::cout << "config exists" << std::endl;

        LoadConfigFile();

        std::cout << "name " << m_name << std::endl;
        std::cout << "author " << m_author << std::endl;
        std::cout << "workspace_path " << m_workspace_path << std::endl;
        std::cout << "time_created " << m_time_created << std::endl;
        std::cout << "last_saved " << m_last_saved << std::endl;
    } else {
        // Create new project and save config
        std::cout << "config not exists" << std::endl;

        const auto current_time = std::chrono::duration_cast<std::chrono::milliseconds>(
                                          std::chrono::system_clock::now().time_since_epoch())
                                          .count();

        m_time_created = current_time;

        m_name = "New Project";
        m_author = "gw";

        SaveConfigFile();
    }
}


void Project::RegisterModule(const std::shared_ptr<Module> &m) {
    m_modules.push_back(m);
    for (const auto &module: m_modules) {
        module->RefreshAllCustomModuleNodes(m);
    }

    if (m->GetGuid() == m_top_level_node_guid)
        m_selected_module = m_modules.size() - 1;
}

std::optional<std::shared_ptr<Module>> Project::GetSelectedModule() {
    if (m_selected_module >= 0 && m_selected_module < m_modules.size()) {
        return m_modules[m_selected_module];
    }
    return std::nullopt;
}
std::optional<std::shared_ptr<Module>> Project::GetModule(const std::string &guid) const {
    for (const auto &module: m_modules) {
        if (module->GetGuid() == guid) {
            return module;
        }
    }
    return std::nullopt;
}

void Project::Save() {
    for (const auto &module: m_modules) {
        CircuitSerializer::SaveModule(this, module);
    }
    SaveConfigFile();
}


void Project::SaveConfigFile() {
    nlohmann::json j_file;

    j_file["name"] = m_name;


    auto dirs = json::array();
    dirs = json::array();

    for (const auto &module: m_modules) {
        const auto &module_path = module->GetName() + ".json";
        dirs.push_back(module_path);
    }


    j_file["author"] = m_author;
    j_file["workspace_path"] = m_workspace_path;

    j_file["selected_module"] = m_selected_module;
    j_file["top_level_node_guid"] = m_top_level_node_guid;
    j_file["time_created"] = m_time_created;
    j_file["module_files"] = dirs;
    j_file["last_saved"] =
            std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch())
                    .count();


    const auto &config_path = m_workspace_path + CONFIG_FILE;

    if (std::ofstream file(config_path); file.is_open()) {
        std::cout << "Writing output file..." << std::endl;
        file << j_file.dump(4);
        file.close();

        ImGui::InsertNotification({ImGuiToastType::Success, 3000, "Saved config to %s", config_path.c_str()});

    } else {
        std::cerr << "Could not open file \"" << config_path << "\"" << std::endl;
        ImGui::InsertNotification({ImGuiToastType::Error, 3000, "Failed to save project config '%s' to %s",
                                   m_name.c_str(), config_path.c_str()});
    }
}


void Project::LoadConfigFile() {
    const auto &config_path = m_workspace_path + CONFIG_FILE;
    std::cout << "loading project " << config_path << std::endl;


    // Open and read config file
    std::ifstream file(config_path);
    std::stringstream buffer;
    buffer << file.rdbuf();

    std::cout << buffer.str() << std::endl;

    // Parse data
    nlohmann::json j = nlohmann::json::parse(buffer.str());

    // Create Module

    m_name = j["name"].get<std::string>();
    m_author = j["author"].get<std::string>();
    m_workspace_path = j["workspace_path"].get<std::string>();
    m_top_level_node_guid = j["top_level_node_guid"].get<std::string>();

    m_time_created = j["time_created"].get<long long>();
    m_last_saved = j["last_saved"].get<long long>();

    m_selected_module = j["selected_module"].get<int>();


    for (auto module_files = j["module_files"].get<std::vector<std::string>>(); const auto &path: module_files) {
        auto m = CircuitSerializer::LoadModule(this, m_workspace_path + "/" + path);
        RegisterModule(m);
    }

    ImGui::InsertNotification({ImGuiToastType::Success, 3000, "Loaded config for project '%s'", m_name.c_str()});
}
