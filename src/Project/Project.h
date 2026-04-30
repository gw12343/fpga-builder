//
// Created by Gabe on 4/28/2026.
//

#pragma once
#include <chrono>
#include <string>


class CopyPasteManager;
class ErrorManager;
class Module;

class Project {
public:
    explicit Project(const std::string &workspace);

    void Render(const std::shared_ptr<ErrorManager> &error_manager,
                const std::shared_ptr<CopyPasteManager> &copy_paste_manager);

    void RegisterModule(const std::shared_ptr<Module> &m);

    [[nodiscard]] std::optional<std::shared_ptr<Module>> GetSelectedModule();
    [[nodiscard]] std::optional<std::shared_ptr<Module>> GetModule(const std::string &guid) const;

    [[nodiscard]] const std::string &GetWorkspacePath() const { return workspace_path; }
    [[nodiscard]] const std::string &GetTopLevelNodeGuid() const { return top_level_node_guid; }

private:
    void SaveConfigFile();
    void LoadConfigFile();


    std::vector<std::shared_ptr<Module>> modules;
    int selected_module = -1;

    long long time_created;
    long long last_saved;

    std::string top_level_node_guid;

    std::string workspace_path;
    std::string name;
    std::string author;
};
