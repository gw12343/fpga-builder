//
// Created by gabed on 4/16/2026.
//

#pragma once
#include <nlohmann/json.hpp>

#include "Default/Node.h"
#include "Link.h"


using json = nlohmann::json;

class CircuitSerializer {
public:
    CircuitSerializer();

    static std::shared_ptr<Module> LoadModule(const std::string &file_path);
    static void SaveModule(const std::shared_ptr<Module> &module, const std::string &file_path);

private:
    static std::unique_ptr<Node> NodeFromJson(const json &j, Module *m);
    static Link LinkFromJson(const json &j, Module *m);
};
