//
// Created by Gabe on 4/26/2026.
//

#include "Toolbox.h"

#include "CircuitSerializer.h"
#include "Codegen/Codegen.h"
#include "Default/AdderNode.h"
#include "Default/BinaryOperator/AndNode.h"
#include "Default/BinaryOperator/NorNode.h"
#include "Default/BinaryOperator/OrNode.h"
#include "Default/BinaryOperator/XOrNode.h"
#include "Default/ClockNode.h"
#include "Default/CombinerNode.h"
#include "Default/CounterNode.h"
#include "Default/DFFNode.h"
#include "Default/DebounceNode.h"
#include "Default/EdgeNode.h"
#include "Default/LiteralNode.h"
#include "Default/MultiplexerNode.h"
#include "Default/RegisterNode.h"
#include "Default/SplitterNode.h"
#include "Default/UnaryOperator/NotNode.h"
#include "IconsFontAwesome6.h"

#define NODE_BTN(name, type)                                                                                           \
    {                                                                                                                  \
        if (ImGui::Button(name, ImVec2(150, 150))) {                                                                   \
            new_node = std::make_shared<type>(module.get());                                                           \
        }                                                                                                              \
    }


void Toolbox::Render(const std::shared_ptr<Module> &module, const std::shared_ptr<ConfigManager> &config_manager) {
    ImGui::Begin("Toolbox");


    std::shared_ptr<Node> new_node;

    NODE_BTN("NOT", NotNode);
    NODE_BTN("OR", OrNode);
    NODE_BTN("NOR", NorNode);
    NODE_BTN("AND", AndNode);
    NODE_BTN("XOR", XOrNode);
    NODE_BTN("MUX", MultiplexerNode);
    NODE_BTN("ADDER", AdderNode);
    NODE_BTN("#", LiteralNode);
    NODE_BTN("SPLITTER", SplitterNode);
    NODE_BTN("COMBINER", CombinerNode);
    NODE_BTN("COUNTER", CounterNode);
    NODE_BTN("REGISTER", RegisterNode);
    NODE_BTN("DFF", DFFNode);
    NODE_BTN("DEBOUNCE", DebounceNode);
    NODE_BTN("EDGE", EdgeNode);
    NODE_BTN(ICON_FA_WAVE_SQUARE, ClockNode);

    if (new_node) {
        config_manager->ConfigureAndAdd(module, new_node);
    }


    ImGui::End();
}
