//
// Created by Gabe on 5/21/2026.
//

#include "CustomModuleNode.h"
void CustomModuleNode::accept(Visitor &v, const int output_slot) { v.visit(*this, output_slot); }

CustomModuleNode::CustomModuleNode(Module *parent) : Node(parent, "Custom") { module_guid = ""; }

CustomModuleNode::CustomModuleNode(Module *parent, const std::string &saved_guid,
                                   const std::string &saved_module_guid) :
    Node(saved_guid, parent, "Custom") {
    module_guid = saved_module_guid;
    CustomModuleNode::InitPinsAfterConfig();
}

void CustomModuleNode::InitPinsAfterConfig() {
    std::cout << "INIT PINS CUSTOM: " << module_guid << std::endl;
    std::cout << "has val: " << module_ref.has_value() << std::endl;
    pins.clear();
    if (!module_ref.has_value())
        return;

    const auto &target_module = module_ref.value();

    int n = 0;
    for (const auto &[name, bits]: target_module->GetInputs()) {
        pins.push_back((Pin){name, ax::NodeEditor::PinKind::Input, *this, n++, PinDataType(bits)});
    }
    for (const auto &[name, bits]: target_module->GetOutputs()) {
        pins.push_back((Pin){name, ax::NodeEditor::PinKind::Output, *this, n++, PinDataType(bits)});
    }
}

std::shared_ptr<Node> CustomModuleNode::Clone() const {
    return std::make_unique<CustomModuleNode>(module, GUID::generate_guid(), module_guid);
}

nlohmann::json CustomModuleNode::ToJson() const {
    nlohmann::json j = Node::ToJson();
    j["module_guid"] = module_guid;
    return j;
}

void CustomModuleNode::RenderConfiguration() {

    if (ImGui::InputText("Module GUID", &module_guid)) {
        module_ref = module->GetProject()->GetModule(module_guid);

        std::cout << "updated node ref to " + (module_ref.has_value() ? module_ref.value()->GetName() : "missing")
                  << std::endl;
    }
}

std::string CustomModuleNode::GetDisplayName() const {
    return module_ref.has_value() ? module_ref.value()->GetName() : "missing";
}
