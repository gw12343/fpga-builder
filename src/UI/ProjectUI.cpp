//
// Created by Gabe on 4/30/2026.
//

#include "Project/Project.h"

#include "Default/CustomModuleNode.h"

void Project::Render(const std::shared_ptr<ErrorManager> &error_manager,
                     const std::shared_ptr<CopyPasteManager> &copy_paste_manager) {
    ImGui::Begin("Project Viewer");
    ImGui::Text("Project Name: '%s'", name.c_str());
    ImGui::Text("Author: %s", author.c_str());
    ImGui::SeparatorText("MODULES");

    int i = 0;
    for (const auto &module: modules) {
        const bool is_top_level = top_level_node_guid == module->guid;


        std::string prefix = is_top_level ? ICON_FA_STAR "  " : "";

        const bool is_selected = i == selected_module;

        const float btn_width = is_top_level ? ImGui::GetContentRegionAvail().x
                                             : ImGui::GetContentRegionAvail().x - ImGui::GetStyle().ItemSpacing.x - 25;

        if (is_selected) {
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.5, 0.1, 0.1, 1.0));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.6, 0.15, 0.15, 1.0));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.6, 0.15, 0.15, 1.0));
        }
        if (ImGui::Button((prefix + module->name).c_str(), ImVec2(btn_width, 0.0f))) {
            selected_module = i;
        }
        if (is_selected) {
            ImGui::PopStyleColor(3);
        }

        if (!is_top_level) {
            ImGui::SameLine();
            if (is_selected) {
                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.15, 0.15, 0.15, 1.0));
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.15, 0.15, 0.15, 1.0));
                ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.15, 0.15, 0.15, 1.0));
            }
            if (ImGui::Button(("+##addmodulenode" + std::to_string(i)).c_str(), ImVec2(25, 0))) {
                if (!is_selected && GetSelectedModule().has_value()) {
                    const auto &sel = GetSelectedModule().value();
                    auto n = std::make_shared<CustomModuleNode>(sel.get(), GUID::generate_guid(), module->guid);
                    n->module_ref = module;
                    n->InitPinsAfterConfig();
                    sel->nodes.push_back(n);
                    std::cout << "making custom node w module guid: " << module->guid << std::endl;
                }
                // if selected module add
            }
            if (is_selected) {
                ImGui::PopStyleColor(3);
            }
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
        ImGui::Text("Select a module to get started."); // TODO center?
    }


    ImGui::End();
    ImGui::PopStyleVar();


    ImGui::End();
}
