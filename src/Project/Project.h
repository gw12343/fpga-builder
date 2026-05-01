//
// Created by Gabe on 4/28/2026.
//

#pragma once

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

    [[nodiscard]] const std::string &GetWorkspacePath() const { return m_workspace_path; }
    [[nodiscard]] const std::string &GetTopLevelNodeGuid() const { return m_top_level_node_guid; }

private:
    void SaveConfigFile();
    void LoadConfigFile();


    std::vector<std::shared_ptr<Module>> m_modules;
    int m_selected_module = -1;

    long long m_time_created;
    long long m_last_saved;

    std::string m_top_level_node_guid;

    std::string m_workspace_path;
    std::string m_name;
    std::string m_author;
};
