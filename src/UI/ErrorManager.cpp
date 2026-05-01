//
// Created by Gabe on 4/19/2026.
//

#include "ErrorManager.h"
#include "Module.h"

#define ERROR_POPUP_TITLE "Error exporting circuit"

void ErrorManager::Render(const std::optional<std::shared_ptr<Module>> &module) {
    if (!module.has_value()) {
        error_present = false;
        error_node_guid = "";
        error_msg = "";
        return;
    }

    if (error_present) {
        ImGui::OpenPopup(ERROR_POPUP_TITLE);
    }

    // Center window
    ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x / 2 - ImGui::GetWindowWidth() / 2,
                                   ImGui::GetIO().DisplaySize.y / 2 - ImGui::GetWindowHeight() / 2));

    if (ImGui::BeginPopupModal(ERROR_POPUP_TITLE, &error_present,
                               ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove)) {
        ImGui::GetStyle().ScaleAllSizes(2.0f);


        ImGui::TextColored(ImVec4(1.0, 0.0, 0.0, 1.0), error_msg.c_str());
        ImGui::Separator();

        if (ImGui::Button("Dismiss", ImVec2(120, 0))) {
            error_present = false;
            error_node_guid = "";
            error_msg = "";
            ImGui::CloseCurrentPopup();
        }
        ImGui::SameLine();

        if (ImGui::Button("View", ImVec2(120, 0))) {
            error_present = false;
            ImGui::CloseCurrentPopup();
        }

        ImGui::GetStyle().ScaleAllSizes(0.5f);

        ImGui::EndPopup();
    }

    if (ImGui::IsKeyDown(ImGuiKey_Escape)) {
        error_present = false;
        error_node_guid = "";
        error_msg = "";
    }
}

void ErrorManager::ThrowError(const std::string &msg, const Node &node) {
    error_present = true;
    error_msg = msg;
    error_node_guid = node.guid;
    ImGui::InsertNotification({ImGuiToastType::Error, 3000, msg.c_str()});
}
