//
// Created by Gabe on 4/19/2026.
//

#include "ErrorManager.h"

#include "Module.h"

#define ERROR_POPUP_TITLE "Error exporting circuit"

void ErrorManager::Render(const std::shared_ptr<Module>& module) {
    if (error_present) {
        ImGui::OpenPopup(ERROR_POPUP_TITLE);
    }

    if (ImGui::BeginPopupModal(ERROR_POPUP_TITLE, &error_present, ImGuiWindowFlags_AlwaysAutoResize)) {
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

            // auto error_node_op = module->GetNode(error_node_guid);
            // if(error_node_op.has_value()) {
            //     Node* error_node = error_node_op.value();
            //
            //     ax::NodeEditor::SelectNode(error_node->id, false);
            //     ax::NodeEditor::NavigateToSelection(true, 0.5);
            // }
        }

        ImGui::EndPopup();
    }

    if(ImGui::IsKeyDown(ImGuiKey_Escape)) {
        error_present = false;
        error_node_guid = "";
        error_msg = "";
    }
}

void ErrorManager::ThrowError(const std::string &msg, const Node &node) {
    error_present = true;
    error_msg = msg;
    error_node_guid = node.guid;
}
