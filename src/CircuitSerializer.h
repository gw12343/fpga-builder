//
// Created by gabed on 4/16/2026.
//

#pragma once
#include <nlohmann/json.hpp>

#include "Default/Node.h"
#include "Link.h"
#include "Project/Project.h"


using json = nlohmann::json;

class CircuitSerializer {
public:
    CircuitSerializer();

    static std::shared_ptr<Module> LoadModule(Project *project, const std::string &file_path);
    static void SaveModule(Project *project, const std::shared_ptr<Module> &module);

private:
    static std::unique_ptr<Node> NodeFromJson(const json &j, Module *m);
    static Link LinkFromJson(const json &j, Module *m);
};
