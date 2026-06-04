//
// Created by gabed on 4/13/2026.
//

#pragma once
#include <stack>

#include "Events/MoveNodesCommand.h"

class Pin;
class Link;
class Node;
class ErrorManager;
class Project;
class Command;

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

class Module : public std::enable_shared_from_this<Module> {
public:
    Module(Project *parent, const std::string &name);
    Module(Project *parent, std::string name, std::string saved_guid);
    ~Module();

    void Render(const std::shared_ptr<ErrorManager> &error_manager);

    bool CreateLink(const Pin &a, const Pin &b);
    void DeleteAllLinksConnected(const std::shared_ptr<Node> &node);

    [[nodiscard]] std::optional<Node *> GetNode(const std::string &guid) const;
    [[nodiscard]] std::optional<Node *> GetNode(const ax::NodeEditor::NodeId &id) const;
    [[nodiscard]] std::optional<Link> GetLink(const ax::NodeEditor::LinkId &id) const;

    std::optional<Pin> GetPin(const std::string &guid);
    std::optional<Pin> GetPin(const ax::NodeEditor::PinId &id);

    void RefreshAllCustomModuleNodes(const std::shared_ptr<Module> &updated_module);

    [[nodiscard]] std::string GetName() const { return m_name; }
    [[nodiscard]] std::string GetGuid() const { return m_guid; }
    [[nodiscard]] Project *GetProject() const { return m_project; }
    [[nodiscard]] std::vector<IO> &GetInputs() { return m_inputs; }
    [[nodiscard]] std::vector<IO> &GetOutputs() { return m_outputs; }

    void AddInput(const IO &io);
    void AddOutput(const IO &io);

    [[nodiscard]] const std::vector<Link> &GetLinks() const { return m_links; }
    [[nodiscard]] const std::vector<std::shared_ptr<Node>> &GetNodes() const { return m_nodes; }

    void RemoveNode(const std::string &guid);
    void AddNode(const std::shared_ptr<Node> &node);
    void AddLink(const Link &link);
    void RemoveLink(ax::NodeEditor::LinkId id);


    void Rename(const std::string &new_name);

    void ExecuteCommand(std::shared_ptr<Command> command);
    void Undo();
    void Redo();

    std::shared_ptr<Module> GetPtr() { return shared_from_this(); }

    std::vector<NodeMove> m_node_moves;

private:
    void RenderUndoRedoList();
    void RenderModuleSettings();
    void RenderNodes(const std::shared_ptr<ErrorManager> &error_manager);
    void RenderLinks() const;


    std::deque<std::shared_ptr<Command>> m_undo_stack;
    std::deque<std::shared_ptr<Command>> m_redo_stack;


    std::vector<std::shared_ptr<Node>> m_copied_nodes;
    std::vector<Link> m_copied_links;


    std::vector<std::shared_ptr<Node>> m_nodes;
    std::vector<Link> m_links;

    Project *m_project;

    std::string m_name;
    std::string m_guid;

    std::vector<IO> m_inputs;
    std::vector<IO> m_outputs;

    ax::NodeEditor::EditorContext *m_context = nullptr;
    ax::NodeEditor::Config *m_config;
};
