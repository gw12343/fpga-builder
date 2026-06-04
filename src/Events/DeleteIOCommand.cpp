//
// Created by Gabe on 6/2/2026.
//

#include "DeleteIOCommand.h"
#include "Module.h"

#include <iostream>


void DeleteIOCommand::execute() {
    if (m_is_input) {
        m_module->GetInputs().pop_back();
    } else {
        m_module->GetOutputs().pop_back();
    }
}

void DeleteIOCommand::undo() {
    if (m_is_input) {
        m_module->GetInputs().push_back(m_io);
    } else {
        m_module->GetOutputs().push_back(m_io);
    }
}

std::string DeleteIOCommand::str() { return "Delete IO: " + m_io.name + "    bits: " + std::to_string(m_io.bits); }
