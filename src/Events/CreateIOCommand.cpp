//
// Created by Gabe on 5/22/2026.
//

#include "CreateIOCommand.h"
#include "Module.h"

#include <iostream>


void CreateIOCommand::execute() {
    if (m_is_input) {
        m_module->GetInputs().push_back(m_io);
    } else {
        m_module->GetOutputs().push_back(m_io);
    }
}

void CreateIOCommand::undo() {
    if (m_is_input) {
        m_module->GetInputs().pop_back();
    } else {
        m_module->GetOutputs().pop_back();
    }
}

std::string CreateIOCommand::str() { return "Create IO: " + m_io.name + "    bits: " + std::to_string(m_io.bits); }
