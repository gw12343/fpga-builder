//
// Created by Gabe on 5/22/2026.
//

#include "CreateNodeCommand.h"
#include "Module.h"

#include <iostream>


void CreateNodeCommand::execute() { m_module->AddNode(m_new_node); }

void CreateNodeCommand::undo() { m_module->RemoveNode(m_new_node->guid); }

std::string CreateNodeCommand::str() { return "Create node: " + m_new_node->name + "    guid: " + m_new_node->guid; }
