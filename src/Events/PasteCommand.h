//
// Created by Gabe on 5/22/2026.
//
#pragma once

#include "Command.h"
#include "Link.h"
#include "Node/Node.h"

#include <vector>

class PasteCommand : public Command {
public:
    PasteCommand(std::shared_ptr<Module> module, std::vector<std::shared_ptr<Node>> nodes, std::vector<Link> links) :
        Command(module), m_nodes(nodes), m_links(links) {}


    void execute() override;
    void undo() override;
    std::string str() override;

private:
    std::vector<std::shared_ptr<Node>> m_nodes;
    std::vector<Link> m_links;
};
