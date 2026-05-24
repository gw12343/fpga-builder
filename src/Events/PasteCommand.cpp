//
// Created by Gabe on 5/22/2026.
//

#include "PasteCommand.h"
#include "Module.h"

#include <iostream>


void PasteCommand::execute() {
    ax::NodeEditor::ClearSelection();

    for (const auto &m_new_node: m_nodes) {
        m_module->AddNode(m_new_node);

        // send to right position?
        // only matters if this is a redo, and
        if (m_new_node->last_pos.x != FLT_MAX && m_new_node->last_pos.x != FLT_MAX) {
            m_new_node->start_pos = m_new_node->last_pos;
            m_new_node->last_pos = {FLT_MAX, FLT_MAX};
        }

        m_new_node->Select(true);
    }


    for (const auto &new_link: m_links) {
        m_module->AddLink(new_link);
    }
}

void PasteCommand::undo() {
    for (const auto &m_new_node: m_nodes) {
        m_module->RemoveNode(m_new_node->guid);
    }

    for (const auto &new_link: m_links) {
        m_module->RemoveLink(new_link.id);
    }
}

std::string PasteCommand::str() {
    return "Paste nodes: " + std::to_string(m_nodes.size()) + "    links: " + std::to_string(m_links.size());
}
