//
// Created by Gabe on 6/2/2026.
//

#include "RenameIOCommand.h"

void RenameIOCommand::execute() {
    if (m_is_input) {
        m_module->GetInputs()[m_io_index].name = m_new_name;
    } else {
        m_module->GetOutputs()[m_io_index].name = m_new_name;
    }
}

void RenameIOCommand::undo() {
    if (m_is_input) {
        m_module->GetInputs()[m_io_index].name = m_old_name;
    } else {
        m_module->GetOutputs()[m_io_index].name = m_old_name;
    }
}

std::string RenameIOCommand::str() { return "Rename IO: " + m_old_name + " to " + m_new_name; }
