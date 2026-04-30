//
// Created by Gabe on 4/28/2026.
//

#pragma once

#include "Module.h"


namespace ax::NodeEditor {
    struct LinkId;
    struct NodeId;
} // namespace ax::NodeEditor


struct CopiedNodeData {
    std::vector<ax::NodeEditor::NodeId> copied_nodes;
    std::vector<ax::NodeEditor::LinkId> selected_nodes;
    std::map<unsigned long long, ImVec2> copied_node_positions;
};

class CopyPasteManager {
public:
    CopyPasteManager() = default;

    void HandleCopyPaste(Module *module, const std::shared_ptr<ErrorManager> &error_manager);

private:
    void CopySelection(Module *module);
    void PasteSelection(Module *module, const std::shared_ptr<ErrorManager> &error_manager);


    std::map<Module *, CopiedNodeData> data;
};
