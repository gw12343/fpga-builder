//
// Created by gabed on 4/13/2026.
//

#pragma once
#include <memory>
#include <optional>
#include <string>
#include <vector>
#include "imgui_node_editor.h"
#include "Link.h"
#include "Default/Node.h"

namespace ed = ax::NodeEditor;

class Module {
public:

    explicit Module(std::string name);
    ~Module();

    void Update();
    void Render(const std::shared_ptr<ErrorManager>& error_manager);


    std::vector<std::string> inputs;
    std::vector<std::string> outputs;

    std::vector<Link> links;

    std::vector<std::unique_ptr<Node>> nodes;

    bool CreateLink(const Pin &a, const Pin &b);

    std::optional<Node*> GetNode(const std::string &guid) const;
    std::optional<Node*> GetNode(const ax::NodeEditor::NodeId &id) const;

    std::optional<Pin> GetPin(const std::string &guid);
    std::optional<Pin> GetPin(const ax::NodeEditor::PinId &id);

    [[nodiscard]] std::string GetName() const { return name; }

private:
    void RenderIOList();
    void RenderNodes(const std::shared_ptr<ErrorManager>& error_manager) const;
    void RenderLinks() const;

    ed::EditorContext* context = nullptr;
    ed::Config config;

    std::string name;

};
