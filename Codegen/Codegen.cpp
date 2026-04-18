//
// Created by gabed on 4/15/2026.
//

#include "Codegen.h"

#include <iostream>
#include <fstream>

#include "Module.h"
#include "Default/BinaryOperator/OrNode.h"
#include "Default/InputNode.h"
#include "Default/LiteralNode.h"
#include "Default/MultiplexerNode.h"
#include "Default/OutputNode.h"

#define OUTPUT_PATH "../EXPORT.v"


void Codegen::GenerateCode(const std::shared_ptr<Module>& module) {
    std::string header = "module " + module->GetName() + " (";
    for (const auto& input : module->inputs) {
        header += "\n\t input wire " + input + ",";
    }
    for (const auto& output : module->outputs) {
        header += "\n\t output wire " + output + ",";   //TODO maybe reg?
    }
    header.pop_back(); // remove last comma
    header += "\n);\n";

    const std::string footer = "endmodule";


    module->nodes[0]->accept(*this);
    std::string eq = returnVals.top();
    returnVals.pop();

    std::string out = header + code + eq + footer;

    if (std::ofstream file(OUTPUT_PATH); file.is_open()) {
        std::cout << "Writing output file..." << std::endl;
        file << out;
        file.close();
    }else {
        std::cerr << "Could not open file \"" << OUTPUT_PATH << "\"" << std::endl;
    }
}

void Codegen::visit(BinaryOpNode &node) {

    const auto a = node.GetAInputPin().GetConnectedPin();
    const auto b = node.GetBInputPin().GetConnectedPin();

    if (!a) {
        CircuitError("Missing connection to input A!", node);
        returnVals.emplace("err");
        return;
    }

    if (!b) {
        CircuitError("Missing connection to input B!", node);
        returnVals.emplace("err");
        return;
    }

    a->GetNode().accept(*this);
    b->GetNode().accept(*this);

    const auto b_val = returnVals.top();
    returnVals.pop();
    const auto a_val = returnVals.top();
    returnVals.pop();


    std::string intermediate_wire_name = GetSafeWireName("bin_op_result");

    code += "wire " + intermediate_wire_name + ";\n";
    //nor g1 (y, a, b); // y = ~(a | b)
    code += node.GetVerilogOperator() + " " + GetSafeWireName(node.GetVerilogOperator()) + " (" + intermediate_wire_name + ", " + a_val + ", " + b_val +");\n";
   // code += "assign "+ intermediate_wire_name + " = " + a_val + " " + node.GetVerilogOperator() + " " + b_val  + ";\n";
    returnVals.emplace(intermediate_wire_name);
}

void Codegen::visit(MultiplexerNode &node) {
    const auto a = node.GetAInputPin().GetConnectedPin();
    const auto b = node.GetBInputPin().GetConnectedPin();
    const auto s = node.GetSelectInputPin().GetConnectedPin();

    if (!a) {
        CircuitError("Missing connection to input A!", node);
        returnVals.emplace("err");
        return;
    }

    if (!b) {
        CircuitError("Missing connection to input B!", node);
        returnVals.emplace("err");
        return;
    }

    if (!s) {
        CircuitError("Missing connection to input Select!", node);
        returnVals.emplace("err");
        return;
    }

    a->GetNode().accept(*this);
    b->GetNode().accept(*this);
    s->GetNode().accept(*this);

    const auto select_val = returnVals.top();
    returnVals.pop();
    const auto b_val = returnVals.top();
    returnVals.pop();
    const auto a_val = returnVals.top();
    returnVals.pop();


    std::string intermediate_wire_name = GetSafeWireName("mux_result");

    code += "wire " + intermediate_wire_name + ";\n";
    code += "assign "+ intermediate_wire_name + " = " + select_val + " ? " + a_val + " : " + b_val  + ";\n";
    returnVals.emplace(intermediate_wire_name);
}

void Codegen::visit(LiteralNode &node) {
    std::string wire_name = GetSafeWireName("number_literal");
    code += "wire " + wire_name + " = 1'b" + std::to_string(node.value) + ";\n";

    returnVals.emplace(wire_name);
}

void Codegen::visit(InputNode &node) {
    returnVals.emplace(node.module->inputs[node.slot]);
}

void Codegen::visit(OutputNode &node) {
    const auto in = node.GetValueInputPin().GetConnectedPin();
    if (!in) {
        CircuitError("Missing connection to output node!", node);
        returnVals.emplace("err");
        return;
    }

    in->GetNode().accept(*this);
    std::string val = returnVals.top();
    returnVals.pop();

    returnVals.emplace("\nassign " + node.module->outputs[node.slot] + " = " + val + ";\n\n");
}

std::string Codegen::GetSafeWireName(const std::string &wire_name) {
    if (wireNameCounts.contains(wire_name)) {
        return wire_name + std::to_string(wireNameCounts[wire_name] ++);
    }else {
        wireNameCounts[wire_name] = 1;
        return wire_name + "0";
    }
}

void Codegen::CircuitError(const std::string& msg, const Node &node) {
    std::cerr << "ERROR EXPORTING CIRCUIT: " << msg << std::endl;
    std::cerr << "related node: " << node.guid << std::endl;
}
