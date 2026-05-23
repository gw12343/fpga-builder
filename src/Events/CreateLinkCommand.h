//
// Created by Gabe on 5/22/2026.
//
#pragma once

#include "Command.h"
#include "Link.h"


class CreateLinkCommand : public Command {
public:
    CreateLinkCommand(std::shared_ptr<Module> module, Link link, std::vector<Link> displaced_links) :
        Command(module), m_link(link), m_displaced_links(displaced_links) {}

    void execute() override;
    void undo() override;
    std::string str() override;

private:
    Link m_link;
    std::vector<Link> m_displaced_links;
};
