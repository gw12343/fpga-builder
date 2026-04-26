//
// Created by Gabe on 4/26/2026.
//

#include "ConfigManager.h"

#include <imgui.h>

#include "Default/Node.h"
#include "Module.h"


#define NODE_CONFIG_TITLE "Configure Node"

void ConfigManager::Render(const std::shared_ptr<Module> &module) {
    if (config_open) {
        ImGui::OpenPopup(NODE_CONFIG_TITLE);
    }

    if (ImGui::BeginPopupModal(NODE_CONFIG_TITLE, &config_open, ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::Text("Configuring new %s node.", current_node->name.c_str());

        current_node->RenderConfiguration();

        ImGui::Separator();
        if (ImGui::Button("Done")) {
            config_open = false;

            current_node->InitPinsAfterConfig();
            module->nodes.push_back(current_node);
            current_node.reset();
        }

        ImGui::EndPopup();
    }
}
void ConfigManager::ConfigureAndAdd(const std::shared_ptr<Module> &module, const std::shared_ptr<Node> &node) {
    if (!node->HasConfiguration()) {
        module->nodes.push_back(node);
        return;
    }

    config_open = true;
    current_node = node;
}
