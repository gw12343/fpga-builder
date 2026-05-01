//
// Created by Gabe on 4/28/2026.
//

#include "CopyPasteManager.h"

#include "Default/ConfigurableDataAndSelectBitWidthNode.h"
#include "Link.h"
#include "Pins/Pin.h"


void CopyPasteManager::CopySelection(Module *module) {
    auto &[copied_nodes, selected_nodes, copied_node_positions] = data[module];


    copied_nodes.resize(ax::NodeEditor::GetSelectedObjectCount());
    selected_nodes.resize(ax::NodeEditor::GetSelectedObjectCount());

    const int node_count = GetSelectedNodes(copied_nodes.data(), static_cast<int>(copied_nodes.size()));
    const int link_count = GetSelectedLinks(selected_nodes.data(), static_cast<int>(selected_nodes.size()));

    copied_nodes.resize(node_count);
    selected_nodes.resize(link_count);
    copied_node_positions.clear();
    for (auto n: copied_nodes) {
        copied_node_positions[n.Get()] = GetNodePosition(n);
    }
}


void CopyPasteManager::PasteSelection(Module *module, const std::shared_ptr<ErrorManager> &error_manager) {
    auto &[copied_nodes, selected_nodes, copied_node_positions] = data[module];

    std::map<std::string, std::string> guid_map;
    std::vector<std::string> old_nodes;


    ax::NodeEditor::ClearSelection();
    // copy each node and put in map
    for (const auto &n: copied_nodes) {
        const auto &curr_node = module->GetNode(n);
        if (!curr_node.has_value())
            continue;

        const auto &curr_node_val = curr_node.value();
        old_nodes.push_back(curr_node_val->guid);
        auto copy = curr_node_val->Clone();

        copy->start_pos = ImVec2(copied_node_positions[n.Get()].x + 40, copied_node_positions[n.Get()].y + 40);
        copy->last_pos = {FLT_MAX, FLT_MAX};

        guid_map[curr_node_val->guid] = copy->guid;

        // Render to init node before selecting
        copy->Render(error_manager);
        ax::NodeEditor::SelectNode(copy->id.Get(), true);

        module->AddNode(copy);
    }

    // get the links that are between only the nodes selected
    std::vector<std::string> old_links;

    struct link_copy_data {
        Link l;
        Node &in;
        Node &out;
    };

    std::vector<link_copy_data> links_to_copy;
    for (const auto &link: module->GetLinks()) {
        const auto &in_pin_op = module->GetPin(link.input_guid);
        const auto &out_pin_op = module->GetPin(link.output_guid);
        if (!in_pin_op || !out_pin_op)
            continue;

        auto &node_in = in_pin_op.value().GetNode();
        auto &node_out = out_pin_op.value().GetNode();

        if (std::ranges::find(old_nodes, node_in.guid) == old_nodes.end())
            continue;
        if (std::ranges::find(old_nodes, node_out.guid) == old_nodes.end())
            continue;

        links_to_copy.push_back({link, node_in, node_out});
    }

    // Create new links between newly copied nodes
    for (const auto &[link, in_node, out_node]: links_to_copy) {
        const auto &new_node_input_id = guid_map[in_node.guid];
        const auto &new_node_output_id = guid_map[out_node.guid];

        const auto &new_node_in_op = module->GetNode(new_node_input_id);
        const auto &new_node_out_op = module->GetNode(new_node_output_id);

        if (!new_node_in_op)
            continue;
        if (!new_node_out_op)
            continue;


        std::string in_suffix =
                link.input_guid.substr(in_node.guid.size(), link.input_guid.size() - in_node.guid.size());

        std::string new_in_id = new_node_in_op.value()->guid + in_suffix;

        std::string out_suffix =
                link.output_guid.substr(out_node.guid.size(), link.output_guid.size() - out_node.guid.size());

        std::string new_out_id = new_node_out_op.value()->guid + out_suffix;


        module->AddLink({module, new_out_id, new_in_id});
    }
}


void CopyPasteManager::HandleCopyPaste(Module *module, const std::shared_ptr<ErrorManager> &error_manager) {
    // If copy
    if (ImGui::GetIO().KeyCtrl && ImGui::IsKeyPressed(ImGuiKey_C)) {
        CopySelection(module);
    } else if (ImGui::GetIO().KeyCtrl && ImGui::IsKeyPressed(ImGuiKey_V)) {
        PasteSelection(module, error_manager);
    }
}
