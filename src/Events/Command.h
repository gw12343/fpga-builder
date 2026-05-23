//
// Created by Gabe on 5/22/2026.
//

#pragma once
#include <memory>

class Module;

class Command {
public:
    Command(std::shared_ptr<Module> module) : m_module(module) {}

    virtual ~Command() = default;

    virtual void execute() {}
    virtual void undo() {}

    virtual std::string str() { return "base command"; }

protected:
    std::shared_ptr<Module> m_module;
};
