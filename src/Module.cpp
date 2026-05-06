//
// Created by gabed on 4/13/2026.
//

#include "Module.h"


#include "CircuitSerializer.h"
#include "Default/CustomModuleNode.h"
#include "Default/Node.h"
#include "GUID.h"
#include "Link.h"


Module::Module(Project *parent, const std::string &name) : Module(parent, name, GUID::generate_guid()) {}

Module::Module(Project *parent, std::string name, std::string saved_guid) :
    m_project(parent), m_name(std::move(name)), m_guid(std::move(saved_guid)) {

    m_config = new ax::NodeEditor::Config();
    m_config->SettingsFile = (name + ".json").c_str();
    m_context = CreateEditor(m_config);
}

Module::~Module() {
    if (m_context) {
        DestroyEditor(m_context);
        m_context = nullptr;
    }
}


bool Module::CreateLink(const Pin &a, const Pin &b) {
    const std::string out_pin_guid = a.GetDirection() == ax::NodeEditor::PinKind::Input ? a.GetGuid() : b.GetGuid();
    const std::string in_pin_guid = a.GetDirection() == ax::NodeEditor::PinKind::Output ? a.GetGuid() : b.GetGuid();

    printf("%s   %s\n", out_pin_guid.c_str(), in_pin_guid.c_str());

    for (const auto &link: m_links) {
        if (link.output_guid == out_pin_guid && link.input_guid == in_pin_guid) {
            // link already exists
            return false;
        }
        if (link.output_guid == out_pin_guid) {
            // remove old connection to this input and replace with current
            std::erase_if(m_links, [&](const Link &l) { return l.id == link.id; });
        }
    }

    if (b.CanConnect(a)) {
        m_links.emplace_back(this, out_pin_guid, in_pin_guid);
        return true;
    }

    return false;
}


void Module::DeleteAllLinksConnected(const std::shared_ptr<Node> &node) {
    for (const auto &pin: node->pins) {
        const std::string &pin_guid = pin.GetGuid();
        std::erase_if(m_links, [&](const Link &l) { return l.input_guid == pin_guid || l.output_guid == pin_guid; });
    }
}

std::optional<Node *> Module::GetNode(const std::string &guid) const {
    for (const auto &node: m_nodes) {
        if (node->guid == guid) {
            return node.get();
        }
    }
    return std::nullopt;
}

std::optional<Node *> Module::GetNode(const ax::NodeEditor::NodeId &id) const {
    for (const auto &node: m_nodes) {
        if (node->id == id) {
            return node.get();
        }
    }
    return std::nullopt;
}

std::optional<Pin> Module::GetPin(const std::string &guid) {
    for (const auto &node: m_nodes) {
        for (auto pin: node->pins) {
            if (pin.GetGuid() == guid) {
                return pin;
            }
        }
    }
    return std::nullopt;
}

std::optional<Pin> Module::GetPin(const ax::NodeEditor::PinId &id) {
    for (const auto &node: m_nodes) {
        for (auto pin: node->pins) {
            if (pin.GetId() == id) {
                return pin;
            }
        }
    }
    return std::nullopt;
}

// TODO call on module io change
void Module::RefreshAllCustomModuleNodes(const std::shared_ptr<Module> &updated_module) {
    for (const auto &node: m_nodes) {
        if (node->GetSerializationType() != "CustomNode")
            continue;

        const auto custom_node = std::dynamic_pointer_cast<CustomModuleNode>(node);
        if (custom_node->module_guid != updated_module->GetGuid())
            continue;

        std::cout << "updating ref in module " << m_name << "  for module guid: " << updated_module->m_name
                  << std::endl;

        custom_node->module_ref = updated_module;
        custom_node->InitPinsAfterConfig();
    }
}


void Module::AddInput(const IO &io) { m_inputs.push_back(io); }
void Module::AddOutput(const IO &io) { m_outputs.push_back(io); }
void Module::AddNode(const std::shared_ptr<Node> &node) { m_nodes.push_back(node); }
void Module::AddLink(const Link &link) { m_links.push_back(link); }

void Module::Rename(const std::string &new_name) {
    CircuitSerializer::RenameModuleFile(m_project, this, new_name);
    m_name = new_name;
}

void Module::RenderNodes(const std::shared_ptr<ErrorManager> &error_manager) const {
    for (const auto &node: m_nodes)
        node->Render(error_manager);
}

void Module::RenderLinks() const {
    for (const auto &link: m_links)
        link.Render();
}
