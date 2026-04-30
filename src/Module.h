//
// Created by gabed on 4/13/2026.
//

#pragma once
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "Default/Node.h"
#include "GUID.h"
#include "Link.h"
#include "imgui_node_editor.h"


class Project;
class CopyPasteManager;
namespace ed = ax::NodeEditor;

struct IO {
    std::string name;
    int bits;
};

class Module {
public:
    Module(Project *parent, std::string name, std::string saved_guid = GUID::generate_guid());
    ~Module();

    void Render(const std::shared_ptr<ErrorManager> &error_manager,
                const std::shared_ptr<CopyPasteManager> &copy_paste_manager);

    std::string name;
    std::string guid;
    Project *project;

    std::vector<IO> inputs;
    std::vector<IO> outputs;

    std::vector<std::shared_ptr<Node>> nodes;
    std::vector<Link> links;

    bool CreateLink(const Pin &a, const Pin &b);
    void DeleteAllLinksConnected(const std::shared_ptr<Node> &node);

    [[nodiscard]] std::optional<Node *> GetNode(const std::string &guid) const;
    [[nodiscard]] std::optional<Node *> GetNode(const ax::NodeEditor::NodeId &id) const;

    std::optional<Pin> GetPin(const std::string &guid);
    std::optional<Pin> GetPin(const ax::NodeEditor::PinId &id);

    void RefreshAllCustomModuleNodes(const std::shared_ptr<Module> &updated_module);

    [[nodiscard]] std::string GetName() const { return name; }
    [[nodiscard]] std::string GetGuid() const { return guid; }

private:
    void RenderModuleSettings();
    void RenderNodes(const std::shared_ptr<ErrorManager> &error_manager) const;
    void RenderLinks() const;

    ed::EditorContext *context = nullptr;
    ed::Config config;
};
