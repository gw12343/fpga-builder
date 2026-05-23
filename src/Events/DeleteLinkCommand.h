//
// Created by Gabe on 5/22/2026.
//
#pragma once

#include "Command.h"
#include "Link.h"


class DeleteLinkCommand : public Command {
public:
    DeleteLinkCommand(std::shared_ptr<Module> module, Link link) :
        Command(module), m_link(link) {}

    void execute() override;
    void undo() override;
    std::string str() override;

private:
    Link m_link;
};
