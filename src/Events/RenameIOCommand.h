//
// Created by Gabe on 6/2/2026.
//

#pragma once

#include "Command.h"
#include "Module.h"


class RenameIOCommand : public Command {
public:
    RenameIOCommand(std::shared_ptr<Module> module, int io_index, bool is_input, std::string old_name,
                    std::string new_name) :
        Command(module), m_io_index(io_index), m_is_input(is_input), m_old_name(old_name), m_new_name(new_name) {}


    void execute() override;
    void undo() override;
    std::string str() override;

private:
    int m_io_index;
    bool m_is_input;
    std::string m_old_name;
    std::string m_new_name;
};
