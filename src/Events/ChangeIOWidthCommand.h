//
// Created by Gabe on 6/4/2026.
//

#pragma once

#include <set>


#include "Command.h"
#include "Module.h"


class ChangeIOWidthCommand final : public Command {
public:
    ChangeIOWidthCommand(const std::shared_ptr<Module> &module, const int io_index, const bool is_input,
                         const int old_size, const int new_size, std::set<Link> displaced_links) :
        Command(module),
        m_io_index(io_index),
        m_is_input(is_input),
        m_old_size(old_size),
        m_new_size(new_size),
        m_displaced_links(displaced_links) {}


    void execute() override;
    void undo() override;
    std::string str() override;

private:
    void UpdateIONodes();

    const bool m_is_input;
    const int m_io_index;
    const int m_old_size;
    const int m_new_size;
    std::set<Link> m_displaced_links;
};
