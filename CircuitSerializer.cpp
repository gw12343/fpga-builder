//
// Created by gabed on 4/16/2026.
//

#include "CircuitSerializer.h"

#include <fstream>
#include <iostream>

#include "Module.h"
#include "Default/InputNode.h"
#include "Default/LiteralNode.h"
#include "Default/MultiplexerNode.h"
#include "Default/OutputNode.h"
#include "Default/BinaryOperator/AndNode.h"
#include "Default/BinaryOperator/OrNode.h"
#include "Default/BinaryOperator/XOrNode.h"


using json = nlohmann::json;

CircuitSerializer::CircuitSerializer() {
}

std::shared_ptr<Module> CircuitSerializer::LoadModule(const std::string &file_path) {
    // Open and read file
    std::ifstream file(file_path);
    std::stringstream buffer;
    buffer << file.rdbuf();
    // Parse data
    json j = json::parse(buffer.str());

    // Create Module
    auto module = std::make_shared<Module>(j["name"].get<std::string>());

    for (json j_nodes = j["nodes"]; const auto& j_node : j_nodes) {
        auto node = node_from_json(j_node, module.get());
        module->nodes.push_back(std::move(node));
    }

    for (json j_links = j["links"]; const auto& j_link : j_links) {
        auto link = link_from_json(j_link, module.get());
        module->links.push_back(link);
    }

    for (json j_inputs = j["inputs"]; const auto& j_in : j_inputs) {
        module->inputs.push_back(j_in.get<std::string>());
    }

    for (json j_outputs = j["outputs"]; const auto& j_out : j_outputs) {
        module->outputs.push_back(j_out.get<std::string>());
    }


    return module;
}

void CircuitSerializer::SaveModule(std::shared_ptr<Module> module, const std::string &file_path) {
    json j_file;
    json j_nodes = json::array();
    json j_links = json::array();
    json j_inputs = json::array();
    json j_outputs = json::array();

    for (const auto& node : module->nodes) {
        json j = node->to_json();
        j_nodes.push_back(j);
    }

    for (const auto& link : module->links) {
        json j = link.to_json();
        j_links.push_back(j);
    }

    for (auto in : module->inputs) {
        json j = in;
        j_inputs.push_back(j);
    }

    for (auto out : module->outputs) {
        json j = out;
        j_outputs.push_back(j);
    }


    j_file["nodes"] = j_nodes;
    j_file["links"] = j_links;
    j_file["inputs"] = j_inputs;
    j_file["outputs"] = j_outputs;
    j_file["name"] = module->GetName();

    if (std::ofstream file("../circuit.json"); file.is_open()) {
        std::cout << "Writing output file..." << std::endl;
        file << j_file.dump(4);
        file.close();
    }else {
        std::cerr << "Could not open file \"" << "circuit.json" << "\"" << std::endl;
    }
}


// Helpers for Node and Link

std::unique_ptr<Node> CircuitSerializer::node_from_json(const json& j, Module* m)
{
    const std::string type = j.at("type");

    std::unique_ptr<Node> p;

    auto guid = j.at("guid").get<std::string>();

    if (type == "OrNode") {
        p = std::make_unique<OrNode>(m, guid);
    }else if (type == "XOrNode") {
        p = std::make_unique<XOrNode>(m, guid);
    }else if (type == "AndNode") {
        p = std::make_unique<AndNode>(m, guid);
    }
    else if (type == "OutputNode") {
        p = std::make_unique<OutputNode>(m, guid, j.at("slot").get<int>());
    }
    else if (type == "InputNode") {
        p = std::make_unique<InputNode>(m, guid, j.at("slot").get<int>());
    }else if (type == "MultiplexerNode") {
        p = std::make_unique<MultiplexerNode>(m, guid);
    }else if (type == "LiteralNode") {
        p = std::make_unique<LiteralNode>(m, guid, j.at("value").get<int>());
    }
    else {
        throw std::runtime_error("Unknown node type: " + type);
    }

    j.at("name").get_to(p->name);
    const int id = j.at("id").get<int>();
    p->id = ax::NodeEditor::NodeId(id);

    const ImVec2 pos(j.at("x").get<float>(), j.at("y").get<float>());

    p->start_pos = ImVec2(pos.x, pos.y);
    p->last_pos = ImVec2(FLT_MAX, FLT_MAX);

    return p;
}


Link CircuitSerializer::link_from_json(const json& j, Module* m)
{
    const std::string input_guid = j.at("input_guid").get<std::string>();
    const std::string output_guid = j.at("output_guid").get<std::string>();
    Link p(m, output_guid, input_guid);


    const int id = j.at("id").get<int>();
    p.id = ax::NodeEditor::LinkId(id);

    return p;
}
