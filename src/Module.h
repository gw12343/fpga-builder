//
// Created by gabed on 4/13/2026.
//

#pragma once
#include <memory>
#include <optional>
#include <string>
#include <vector>
#include "Default/Node.h"
#include "Link.h"
#include "imgui_node_editor.h"


class CopyPasteManager;
namespace ed = ax::NodeEditor;

struct IO {
    std::string name;
    int bits;
};

class Module {
public:
    explicit Module(std::string name);
    ~Module();

    void Render(const std::shared_ptr<ErrorManager> &error_manager,
                const std::shared_ptr<CopyPasteManager> &copy_paste_manager);


    std::vector<IO> inputs;
    std::vector<IO> outputs;

    std::vector<Link> links;

    std::vector<std::shared_ptr<Node>> nodes;
    std::string name;

    bool CreateLink(const Pin &a, const Pin &b);
    void DeleteAllLinksConnected(const std::shared_ptr<Node> &node);

    [[nodiscard]] std::optional<Node *> GetNode(const std::string &guid) const;
    [[nodiscard]] std::optional<Node *> GetNode(const ax::NodeEditor::NodeId &id) const;

    std::optional<Pin> GetPin(const std::string &guid);
    std::optional<Pin> GetPin(const ax::NodeEditor::PinId &id);

    [[nodiscard]] std::string GetName() const { return name; }

private:
    void RenderModuleSettings();
    void RenderNodes(const std::shared_ptr<ErrorManager> &error_manager) const;
    void RenderLinks() const;

    ed::EditorContext *context = nullptr;
    ed::Config config;
};
