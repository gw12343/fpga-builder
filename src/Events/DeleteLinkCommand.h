//
// Created by Gabe on 5/22/2026.
//
#pragma once

#include "Command.h"
#include "Link.h"


class DeleteLinkCommand : public Command {
public:
    DeleteLinkCommand(std::shared_ptr<Module> module, std::vector<Link> links) : Command(module), m_links(links) {}

    void execute() override;
    void undo() override;
    std::string str() override;

private:
    std::vector<Link> m_links;
};
