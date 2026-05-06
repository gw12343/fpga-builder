//
// Created by gabed on 4/14/2026.
//


#define IMGUI_DEFINE_MATH_OPERATORS

#include "Node.h"

#include "GUID.h"
#include "Pins/Pin.h"


Node::Node(Module *parent, const std::string &name, const std::vector<PinCreationData> &inputs,
           const std::vector<PinCreationData> &outputs) :
    Node(GUID::generate_guid(), parent, name, inputs, outputs) {}


Node::Node(std::string saved_guid, Module *parent, std::string in_name, const std::vector<PinCreationData> &inputs,
           const std::vector<PinCreationData> &outputs) {
    module = parent;
    guid = std::move(saved_guid);
    id = GUID::to_id(guid);
    name = std::move(in_name);

    InitPins(inputs, outputs);
}

// Non pin-creating constructors
Node::Node(Module *parent, const std::string &name) : Node(GUID::generate_guid(), parent, name) {}

Node::Node(std::string saved_guid, Module *parent, std::string name) : name(std::move(name)) {
    module = parent;
    guid = std::move(saved_guid);
    id = GUID::to_id(guid);
}


void Node::InitPins(const std::vector<PinCreationData> &inputs, const std::vector<PinCreationData> &outputs) {
    int i = 0;
    for (const auto &[name, type]: inputs) {
        Pin new_input(name, ax::NodeEditor::PinKind::Input, *this, i++, type);
        pins.push_back(new_input);
    }

    for (const auto &[name, type]: outputs) {
        Pin new_output(name, ax::NodeEditor::PinKind::Output, *this, i++, type);
        pins.push_back(new_output);
    }
}


std::optional<Pin> Node::FindPin(const std::string &name) {
    for (const auto &pin: pins) {
        if (pin.GetName() == name)
            return pin;
    }
    return std::nullopt;
}
