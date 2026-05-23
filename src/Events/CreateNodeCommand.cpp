//
// Created by Gabe on 5/22/2026.
//

#include "CreateNodeCommand.h"
#include "Module.h"

#include <iostream>


void CreateNodeCommand::execute() {
    m_module->AddNode(m_new_node);

    // send to right position?
    // only matters if this is a redo, and
    if (m_new_node->last_pos.x != FLT_MAX && m_new_node->last_pos.x != FLT_MAX) {
        m_new_node->start_pos = m_new_node->last_pos;
        m_new_node->last_pos = {FLT_MAX, FLT_MAX};
    }
}

void CreateNodeCommand::undo() { m_module->RemoveNode(m_new_node->guid); }

std::string CreateNodeCommand::str() { return "Create node: " + m_new_node->name + "    guid: " + m_new_node->guid; }
