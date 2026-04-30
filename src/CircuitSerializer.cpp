//
// Created by gabed on 4/16/2026.
//

#include "CircuitSerializer.h"

#include <fstream>
#include <iostream>

#include "Default/AdderNode.h"
#include "Default/BinaryOperator/AndNode.h"
#include "Default/BinaryOperator/NorNode.h"
#include "Default/BinaryOperator/OrNode.h"
#include "Default/BinaryOperator/XOrNode.h"
#include "Default/ClockNode.h"
#include "Default/CombinerNode.h"
#include "Default/CounterNode.h"
#include "Default/CustomModuleNode.h"
#include "Default/DFFNode.h"
#include "Default/DebounceNode.h"
#include "Default/EdgeNode.h"
#include "Default/InputNode.h"
#include "Default/LiteralNode.h"
#include "Default/MultiplexerNode.h"
#include "Default/OutputNode.h"
#include "Default/RegisterNode.h"
#include "Default/SplitterNode.h"
#include "Default/UnaryOperator/NotNode.h"
#include "Module.h"
#include "Project/Project.h"


using json = nlohmann::json;


// Helpers for Node and Link

std::unique_ptr<Node> CircuitSerializer::NodeFromJson(const json &j, Module *m) {
    const std::string type = j.at("type");

    std::unique_ptr<Node> p;

    auto guid = j.at("guid").get<std::string>();

    if (type == "OrNode") {
        p = std::make_unique<OrNode>(m, guid, j.at("bits").get<int>());
    } else if (type == "CustomNode") {
        p = std::make_unique<CustomModuleNode>(m, guid, j.at("module_guid").get<std::string>());
    } else if (type == "XOrNode") {
        p = std::make_unique<XOrNode>(m, guid, j.at("bits").get<int>());
    } else if (type == "NorNode") {
        p = std::make_unique<NorNode>(m, guid, j.at("bits").get<int>());
    } else if (type == "AndNode") {
        p = std::make_unique<AndNode>(m, guid, j.at("bits").get<int>());
    } else if (type == "OutputNode") {
        p = std::make_unique<OutputNode>(m, guid, j.at("slot").get<int>());
    } else if (type == "InputNode") {
        p = std::make_unique<InputNode>(m, guid, j.at("slot").get<int>());
    } else if (type == "MultiplexerNode") {
        p = std::make_unique<MultiplexerNode>(m, guid, j.at("data_bits").get<int>(), j.at("select_bits").get<int>());
    } else if (type == "LiteralNode") {
        p = std::make_unique<LiteralNode>(m, guid, j.at("bits").get<int>(), j.at("value").get<int>());
    } else if (type == "SplitterNode") {
        p = std::make_unique<SplitterNode>(m, guid, j.at("bits").get<int>());
    } else if (type == "CombinerNode") {
        p = std::make_unique<CombinerNode>(m, guid, j.at("bits").get<int>());
    } else if (type == "CounterNode") {
        p = std::make_unique<CounterNode>(m, guid, j.at("bits").get<int>());
    } else if (type == "RegisterNode") {
        p = std::make_unique<RegisterNode>(m, guid, j.at("bits").get<int>());
    } else if (type == "DFFNode") {
        p = std::make_unique<DFFNode>(m, guid);
    } else if (type == "DebounceNode") {
        p = std::make_unique<DebounceNode>(m, guid);
    } else if (type == "EdgeNode") {
        p = std::make_unique<EdgeNode>(m, guid);
    } else if (type == "AdderNode") {
        p = std::make_unique<AdderNode>(m, guid, j.at("bits").get<int>());
    } else if (type == "ClockNode") {
        p = std::make_unique<ClockNode>(m, guid);
    } else if (type == "NotNode") {
        p = std::make_unique<NotNode>(m, guid, j.at("bits").get<int>());
    } else {
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


Link CircuitSerializer::LinkFromJson(const json &j, Module *m) {
    const std::string input_guid = j.at("input_guid").get<std::string>();
    const std::string output_guid = j.at("output_guid").get<std::string>();
    Link p(m, output_guid, input_guid);


    const int id = j.at("id").get<int>();
    p.id = ax::NodeEditor::LinkId(id);

    return p;
}


// other

CircuitSerializer::CircuitSerializer() = default;

std::shared_ptr<Module> CircuitSerializer::LoadModule(Project *project, const std::string &file_path) {
    std::cout << "loading module " << file_path << std::endl;

    // Open and read file
    std::ifstream file(file_path);
    std::stringstream buffer;
    buffer << file.rdbuf();
    // Parse data
    json j = json::parse(buffer.str());

    // Create Module
    auto module = std::make_shared<Module>(project, j["name"].get<std::string>(), j["guid"].get<std::string>());

    for (json j_inputs = j["inputs"]; const auto &j_in: j_inputs) {
        module->inputs.push_back({j_in["name"].get<std::string>(), j_in["bits"].get<int>()});
    }

    for (json j_outputs = j["outputs"]; const auto &j_out: j_outputs) {
        module->outputs.push_back({j_out["name"].get<std::string>(), j_out["bits"].get<int>()});
    }

    for (json j_nodes = j["nodes"]; const auto &j_node: j_nodes) {
        auto node = NodeFromJson(j_node, module.get());
        module->nodes.push_back(std::move(node));
    }

    for (json j_links = j["links"]; const auto &j_link: j_links) {
        auto link = LinkFromJson(j_link, module.get());
        module->links.push_back(link);
    }


    ImGui::InsertNotification({ImGuiToastType::Success, 3000, "Loaded module '%s'", module->name.c_str()});

    return module;
}

void CircuitSerializer::SaveModule(Project *project, const std::shared_ptr<Module> &module) {
    json j_file;
    json j_nodes = json::array();
    json j_links = json::array();
    json j_inputs = json::array();
    json j_outputs = json::array();

    for (const auto &node: module->nodes) {
        json j = node->ToJson();
        j_nodes.push_back(j);
    }

    for (const auto &link: module->links) {
        json j = link.to_json();
        j_links.push_back(j);
    }

    for (const auto &[name, bits]: module->inputs) {
        json j;
        j["name"] = name;
        j["bits"] = bits;
        j_inputs.push_back(j);
    }

    for (const auto &[name, bits]: module->outputs) {
        json j;
        j["name"] = name;
        j["bits"] = bits;
        j_outputs.push_back(j);
    }


    j_file["nodes"] = j_nodes;
    j_file["links"] = j_links;
    j_file["inputs"] = j_inputs;
    j_file["outputs"] = j_outputs;
    j_file["name"] = module->GetName();
    j_file["guid"] = module->GetGuid();

    const std::string file_path = project->GetWorkspacePath() + "/" + module->GetName() + ".json";

    if (std::ofstream file(file_path); file.is_open()) {
        std::cout << "Writing output file..." << std::endl;
        file << j_file.dump(4);
        file.close();
        ImGui::InsertNotification(
                {ImGuiToastType::Success, 3000, "Saved module '%s' to %s", module->name.c_str(), file_path.c_str()});
    } else {
        std::cerr << "Could not open file \"" << file_path << "\"" << std::endl;
        ImGui::InsertNotification({ImGuiToastType::Error, 3000, "Failed to save module '%s' to %s",
                                   module->name.c_str(), file_path.c_str()});
    }
}
