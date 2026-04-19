//
// Created by gabed on 4/15/2026.
//

#include "Codegen.h"

#include <iostream>
#include <fstream>

#include "Module.h"
#include "Default/DebounceNode.h"
#include "Default/DFFNode.h"
#include "Default/BinaryOperator/OrNode.h"
#include "Default/InputNode.h"
#include "Default/LiteralNode.h"
#include "Default/MultiplexerNode.h"
#include "Default/OutputNode.h"

#define OUTPUT_PATH "../EXPORT.v"



#define CHECK_CACHE {auto memo = CheckCache(node.guid);if(memo.has_value()){returnVals.emplace(memo.value()); std::cout << "used cache: " << node.name << std::endl; return;}}
#define CACHE_AND_RETURN(node, val) { visitedNodes[node.guid] = val; returnVals.emplace(val); return; }


#define ERROR_AND_RETURN { visitedNodes[node.guid] = "err"; returnVals.emplace("err"); return; }

#define VERIFY_CONNECTION(PIN) {if (!PIN) { CircuitError("Missing connection to input " #PIN "!", node); ERROR_AND_RETURN }}


#define START_CHECK_CYCLES { if(CheckActive(node.guid)){ CircuitError("Cycle detected!", node); ERROR_AND_RETURN} activeNodes.emplace(node.guid);}
#define END_CHECK_CYCLES activeNodes.pop();


void Codegen::GenerateCode(const std::shared_ptr<Module>& module) {
    std::string header = "module " + module->GetName() + " (";
    for (const auto& input : module->inputs) {
        header += "\n\t input wire " + input + ",";
    }
    for (const auto& output : module->outputs) {
        header += "\n\t output reg " + output + ",";   //TODO maybe reg?
    }
    header.pop_back(); // remove last comma
    header += "\n);\n";

    const std::string footer = "endmodule";


    module->nodes[0]->accept(*this);
    returnVals.pop();

    std::string out =
            header +
            "\n// ─── wire/reg declarations ────────────────────────────────\n" +
            decls +
            "\n// ─── combination logic ────────────────────────────────────\n" +

            "\talways @(*) begin\n\n" +
            inner + "\n\n" +
            "\tend\n\n" +

            later +
            footer;

    if (std::ofstream file(OUTPUT_PATH); file.is_open()) {
        std::cout << "Writing output file..." << std::endl;
        file << out;
        file.close();
    }else {
        std::cerr << "Could not open file \"" << OUTPUT_PATH << "\"" << std::endl;
    }
}




void Codegen::visit(DebounceNode &node) {
    CHECK_CACHE

    std::string output_reg = GetSafeWireName("debounce_out");
    visitedNodes[node.guid] = output_reg;

    // Input pin
    const auto d = node.GetDPin().GetConnectedPin();
    const auto clk = node.GetClockPin().GetConnectedPin();

    VERIFY_CONNECTION(d);
    VERIFY_CONNECTION(clk);

    auto d_val = EvalNode(d->GetNode());
    auto clk_val = EvalNode(clk->GetNode());


    // declare output register and shift register
    std::string output_sr = GetSafeWireName("debounce_sr");
    decls += "reg [3:0]" + output_sr + ";\n";
    decls += "reg     " + output_reg + ";\n";




    // debounce shift block
    later += "\talways @(posedge " + clk_val + ") begin\n";
    later += "\t\t" + output_sr + " <= { " + output_sr + "[2:0], " + d_val + " };\n";
    later += "\tend\n\n";

    // debounce output register set
    later += "\talways @(posedge " + clk_val + ") begin\n";
    later += "\t\tif (" + output_sr + " == 4'b1111)\n";
    later += "\t\t\t" + output_reg + " <= 1'b1;\n";
    later += "\t\telse if (" + output_sr + " == 4'b0000)\n";
    later += "\t\t\t" + output_reg + " <= 1'b0;\n";
    later += "\tend\n\n";


    CACHE_AND_RETURN(node, output_reg)

}

void Codegen::visit(DFFNode &node) {
    CHECK_CACHE
    //START_CHECK_CYCLES

    std::string output_reg = GetSafeWireName("dff_out");
    visitedNodes[node.guid] = output_reg;

    // Input pins
    const auto set = node.GetSetPin().GetConnectedPin();
    const auto rst = node.GetResetPin().GetConnectedPin();
    const auto d = node.GetDPin().GetConnectedPin();
    const auto clk = node.GetClkPin().GetConnectedPin();

    // Verify connections
    VERIFY_CONNECTION(set);
    VERIFY_CONNECTION(rst);
    VERIFY_CONNECTION(d);
    VERIFY_CONNECTION(clk);

    auto set_val = EvalNode(set->GetNode());
    auto rst_val = EvalNode(rst->GetNode());
    auto d_val = EvalNode(d->GetNode());
    auto clk_val = EvalNode(clk->GetNode());

    // declare output register
    decls += "reg " + output_reg + ";\n";

    // Declare clocked logic
    later += "\talways @(posedge " + clk_val + ") begin\n";
    later += "\t\tif (" + rst_val + ")\n";
    later += "\t\t\t" + output_reg + " <= 1'b0;\n"; // TODO add option for reset value to node
    later += "\t\telse if (" + set_val + ")\n";
    later += "\t\t\t" + output_reg + " <= " + d_val + ";\n";
    later += "\tend\n\n";

    //END_CHECK_CYCLES
    CACHE_AND_RETURN(node, output_reg)
}


void Codegen::visit(BinaryOpNode &node) {
    CHECK_CACHE
    START_CHECK_CYCLES

    const auto a = node.GetAInputPin().GetConnectedPin();
    const auto b = node.GetBInputPin().GetConnectedPin();

    VERIFY_CONNECTION(a);
    VERIFY_CONNECTION(b);

    const auto b_val = EvalNode(b->GetNode());
    const auto a_val = EvalNode(a->GetNode());



    std::string intermediate_wire_name = GetSafeWireName("bin_op_result");

    decls += "reg " + intermediate_wire_name + ";\n";
    inner += node.GetVerilogAssign(intermediate_wire_name, a_val, b_val);

    END_CHECK_CYCLES
    CACHE_AND_RETURN(node, intermediate_wire_name)
}


void Codegen::visit(MultiplexerNode &node) {
    CHECK_CACHE
    START_CHECK_CYCLES

    const auto a = node.GetAInputPin().GetConnectedPin();
    const auto b = node.GetBInputPin().GetConnectedPin();
    const auto s = node.GetSelectInputPin().GetConnectedPin();

    VERIFY_CONNECTION(a);
    VERIFY_CONNECTION(b);
    VERIFY_CONNECTION(s);

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
    decls += "reg " + intermediate_wire_name + ";\n";

    inner += "\tif(" + select_val + ")\n\t\t";
    inner += intermediate_wire_name + " = " + a_val + ";\n";
    inner += "\telse\n\t\t";
    inner += intermediate_wire_name + " = " + b_val + ";\n\n";

    END_CHECK_CYCLES
    CACHE_AND_RETURN(node, intermediate_wire_name)
}

void Codegen::visit(LiteralNode &node) {
    CHECK_CACHE
    START_CHECK_CYCLES

    std::string wire_name = GetSafeWireName("number_literal");
    decls += "wire " + wire_name + " = 1'b" + std::to_string(node.value) + ";\n";

    END_CHECK_CYCLES
    CACHE_AND_RETURN(node, wire_name)
}

void Codegen::visit(InputNode &node) {
    CHECK_CACHE
    START_CHECK_CYCLES

    END_CHECK_CYCLES
    CACHE_AND_RETURN(node, node.module->inputs[node.slot])
}

void Codegen::visit(OutputNode &node) {
    CHECK_CACHE
    START_CHECK_CYCLES

    const auto in = node.GetValueInputPin().GetConnectedPin();
    VERIFY_CONNECTION(in);

    in->GetNode().accept(*this);
    std::string val = returnVals.top();
    returnVals.pop();

    inner += "\n" + node.module->outputs[node.slot] + " = " + val + ";\n\n";

    END_CHECK_CYCLES
    CACHE_AND_RETURN(node, "");
}



std::optional<std::string> Codegen::CheckCache(const std::string& guid) {
    if(!visitedNodes.contains(guid)) return std::nullopt;

    return visitedNodes[guid];
}

std::string Codegen::GetSafeWireName(const std::string &wire_name) {
    if (wireNameCounts.contains(wire_name)) {
        return wire_name + std::to_string(wireNameCounts[wire_name] ++);
    }

    wireNameCounts[wire_name] = 1;
    return wire_name + "0";
}

void Codegen::CircuitError(const std::string& msg, const Node &node) {
    std::cerr << "ERROR EXPORTING CIRCUIT: " << msg << std::endl;
    std::cerr << "related node: " << node.guid << std::endl;
}

bool Codegen::CheckActive(const std::string &guid) {
    std::stack<std::string> tmp;
    bool found = false;

    while(!activeNodes.empty()){
        if(activeNodes.top() == guid){
            found = true;
            break;
        }
        tmp.push(activeNodes.top());
        activeNodes.pop();
    }

    while(!tmp.empty()){
        activeNodes.push(tmp.top());
        tmp.pop();
    }
    return found;
}
