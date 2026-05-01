//
// Created by gabed on 4/13/2026.
//

#pragma once
#include <intrin.h>


class Pin;
class Link;
class Node;
class ErrorManager;
class Project;
class CopyPasteManager;


namespace ax::NodeEditor {
    struct Config;
    struct EditorContext;
    struct PinId;
    struct NodeId;
} // namespace ax::NodeEditor

struct IO {
    std::string name;
    int bits;
};

class Module {
public:
    Module(Project *parent, const std::string &name);
    Module(Project *parent, std::string name, std::string saved_guid);
    ~Module();

    void Render(const std::shared_ptr<ErrorManager> &error_manager,
                const std::shared_ptr<CopyPasteManager> &copy_paste_manager);

    bool CreateLink(const Pin &a, const Pin &b);
    void DeleteAllLinksConnected(const std::shared_ptr<Node> &node);

    [[nodiscard]] std::optional<Node *> GetNode(const std::string &guid) const;
    [[nodiscard]] std::optional<Node *> GetNode(const ax::NodeEditor::NodeId &id) const;

    std::optional<Pin> GetPin(const std::string &guid);
    std::optional<Pin> GetPin(const ax::NodeEditor::PinId &id);

    void RefreshAllCustomModuleNodes(const std::shared_ptr<Module> &updated_module);

    [[nodiscard]] std::string GetName() const { return name; }
    [[nodiscard]] std::string GetGuid() const { return guid; }
    [[nodiscard]] Project *GetProject() const { return project; }
    [[nodiscard]] const std::vector<IO> &GetInputs() const { return inputs; }
    [[nodiscard]] const std::vector<IO> &GetOutputs() const { return outputs; }

    void AddInput(const IO &io);
    void AddOutput(const IO &io);

    [[nodiscard]] const std::vector<Link> &GetLinks() const { return links; }
    [[nodiscard]] const std::vector<std::shared_ptr<Node>> &GetNodes() const { return nodes; }

    void AddNode(const std::shared_ptr<Node> &node);
    void AddLink(const Link &link);
    void Rename(const std::string &new_name);

private:
    void RenderModuleSettings();
    void RenderNodes(const std::shared_ptr<ErrorManager> &error_manager) const;
    void RenderLinks() const;

    std::vector<std::shared_ptr<Node>> nodes;
    std::vector<Link> links;

    Project *project;

    std::string name;
    std::string guid;

    std::vector<IO> inputs;
    std::vector<IO> outputs;

    ax::NodeEditor::EditorContext *context = nullptr;
    ax::NodeEditor::Config *config;
};
