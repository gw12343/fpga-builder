//
// Created by Gabe on 4/26/2026.
//

#include "ConfigManager.h"
#include "Default/Node.h"
#include "Module.h"


#define NODE_CONFIG_TITLE "Configure Node"

void ConfigManager::Render(const std::optional<std::shared_ptr<Module>> &module) {
    if (!module.has_value()) {
        m_config_open = false;
        return;
    }

    if (m_config_open) {
        ImGui::OpenPopup(NODE_CONFIG_TITLE);
    }

    // Center window
    ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x / 2 - ImGui::GetWindowWidth() / 2,
                                   ImGui::GetIO().DisplaySize.y / 2 - ImGui::GetWindowHeight() / 2));

    if (ImGui::BeginPopupModal(NODE_CONFIG_TITLE, &m_config_open,
                               ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove)) {
        ImGui::GetStyle().ScaleAllSizes(2.0f);
        // ImGui::SetWindowFontScale(2.0f);
        ImGui::Text("Configuring new %s node.", m_current_node->name.c_str());

        m_current_node->RenderConfiguration();

        ImGui::Separator();
        if (ImGui::Button("Done", ImVec2(100, 40))) {
            m_config_open = false;

            ImGui::InsertNotification({ImGuiToastType::Success, 3000, "Configured and added new node: %s",
                                       m_current_node->GetDisplayName().c_str()});

            m_current_node->InitPinsAfterConfig();
            module.value()->AddNode(m_current_node);
            m_current_node.reset();
        }

        ImGui::SameLine(145);

        if (ImGui::Button("Cancel", ImVec2(100, 40)) || ImGui::IsKeyPressed(ImGuiKey_Escape)) {
            m_config_open = false;
        }
        // ImGui::SetWindowFontScale(1.0f);
        ImGui::GetStyle().ScaleAllSizes(0.5f);
        ImGui::EndPopup();
    }
}
void ConfigManager::ConfigureAndAdd(const std::shared_ptr<Module> &module, const std::shared_ptr<Node> &node) {
    if (!node->HasConfiguration()) {
        ImGui::InsertNotification(
                {ImGuiToastType::Success, 3000, "Added new node: %s", node->GetDisplayName().c_str()});
        module->AddNode(node);
        return;
    }

    m_config_open = true;
    m_current_node = node;
}
