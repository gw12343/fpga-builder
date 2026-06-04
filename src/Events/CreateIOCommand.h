//
// Created by Gabe on 5/22/2026.
//
#pragma once

#include "Command.h"
#include "Module.h"


class CreateIOCommand : public Command {
public:
    CreateIOCommand(std::shared_ptr<Module> module, IO io, bool is_input) :
        Command(module), m_io(io), m_is_input(is_input) {}


    void execute() override;
    void undo() override;
    std::string str() override;

private:
    IO m_io;
    bool m_is_input;
};
