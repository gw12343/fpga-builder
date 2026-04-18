//
// Created by gabed on 4/15/2026.
//

#include "TraversePrint.h"

#include <iostream>

#include "Module.h"
#include "Default/InputNode.h"
#include "Default/LiteralNode.h"
#include "Default/MultiplexerNode.h"
#include "Default/OutputNode.h"
#include "Default/BinaryOperator/OrNode.h"


static int indent = 0;

void TraversePrint::visit(BinaryOpNode &node) {
    std::cout << std::string(indent * 3, ' ').c_str() << "Visiting " << node.name << std::endl;

    indent ++;

    auto a = node.GetAInputPin();
    auto b = node.GetBInputPin();

    auto ac = a.GetConnectedPin();
    auto bc = b.GetConnectedPin();


    if (ac) {
        ac->GetNode().accept(*this);
    }

    if (bc) {
        bc->GetNode().accept(*this);
    }


    indent--;


}

void TraversePrint::visit(MultiplexerNode &node) {
    std::cout << std::string(indent * 3, ' ').c_str() << "Visiting " << node.name << std::endl;

    indent ++;

    auto a = node.GetAInputPin();
    auto b = node.GetBInputPin();
    auto s = node.GetSelectInputPin();

    auto ac = a.GetConnectedPin();
    auto bc = b.GetConnectedPin();
    auto sc = s.GetConnectedPin();


    if (ac) {
        ac->GetNode().accept(*this);
    }

    if (bc) {
        bc->GetNode().accept(*this);
    }

    if (sc) {
        sc->GetNode().accept(*this);
    }


    indent--;


}

void TraversePrint::visit(LiteralNode &node) {
    std::cout << std::string(indent * 3, ' ').c_str() << "Visiting " << node.name << "(" << node.value << ")" << std::endl;
}

void TraversePrint::visit(InputNode &node) {
    std::cout << std::string(indent * 3, ' ').c_str() << "Visiting " << node.name << "(" << node.module->inputs[node.slot] << ")" << std::endl;
}

void TraversePrint::visit(OutputNode &node) {
    std::cout << std::string(indent * 3, ' ').c_str() << "Visiting " << node.name << "(" << node.module->outputs[node.slot] << ")" << std::endl;


    auto pin = node.GetValueInputPin();

    if (const auto connected = pin.GetConnectedPin()) {
        indent++;
        connected->GetNode().accept(*this);
        indent--;

    }else {
        printf("no connection\n");
    }

}
