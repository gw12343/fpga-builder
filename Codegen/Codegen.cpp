//
// Created by gabed on 4/15/2026.
//

#include "Codegen.h"

#include <fstream>
#include <iostream>
#include <utility>

#include "Default/BinaryOperator/OrNode.h"
#include "Default/ClockNode.h"
#include "Default/CombinerNode.h"
#include "Default/CounterNode.h"
#include "Default/DFFNode.h"
#include "Default/DebounceNode.h"
#include "Default/EdgeNode.h"
#include "Default/InputNode.h"
#include "Default/LiteralNode.h"
#include "Default/MultiplexerNode.h"
#include "Default/OutputNode.h"
#include "Default/RegisterNode.h"
#include "Default/SplitterNode.h"
#include "Default/UnaryOperator/UnaryOpNode.h"
#include "Module.h"

#define OUTPUT_PATH "../EXPORT.v"


#define NODE_KEY(n) (node.guid + "-slot-" + std::to_string(n))


#define CHECK_CACHE                                                                                                    \
    {                                                                                                                  \
        auto memo = CheckCache(NODE_KEY(output_slot));                                                                 \
        if (memo.has_value()) {                                                                                        \
            returnVals.emplace(memo.value());                                                                          \
            std::cout << "used cache: " << node.name << std::endl;                                                     \
            return;                                                                                                    \
        }                                                                                                              \
    }
#define CACHE_AND_RETURN(node, val, slot)                                                                              \
    {                                                                                                                  \
        visitedNodes[NODE_KEY(slot)] = val;                                                                            \
        returnVals.emplace(val);                                                                                       \
        return;                                                                                                        \
    }


#define RETURN_REG(val)                                                                                                \
    {                                                                                                                  \
        returnVals.emplace(val);                                                                                       \
        return;                                                                                                        \
    }

#define ERROR_AND_RETURN                                                                                               \
    {                                                                                                                  \
        visitedNodes[NODE_KEY(output_slot)] = "err";                                                                   \
        returnVals.emplace("err");                                                                                     \
        return;                                                                                                        \
    }

#define VERIFY_CONNECTION(PIN)                                                                                         \
    {                                                                                                                  \
        if (!PIN) {                                                                                                    \
            CircuitError("Missing connection to input " #PIN "!", node);                                               \
            ERROR_AND_RETURN                                                                                           \
        }                                                                                                              \
    }


#define START_CHECK_CYCLES                                                                                             \
    {                                                                                                                  \
        if (CheckActive(node.guid)) {                                                                                  \
            CircuitError("Cycle detected!", node);                                                                     \
            ERROR_AND_RETURN                                                                                           \
        }                                                                                                              \
        activeNodes.emplace(node.guid);                                                                                \
    }
#define END_CHECK_CYCLES activeNodes.pop();


Codegen::Codegen(std::shared_ptr<ErrorManager> error_man) : error_manager(std::move(error_man)) {}

void Codegen::GenerateCode(const std::shared_ptr<Module> &module) {
    std::string header = "module " + module->GetName() + " (";
    for (const auto &input: module->inputs) {
        header += "\n\t input wire " + input + ",";
    }
    header += "\n\tinput wire sys_clk,";
    for (const auto &output: module->outputs) {
        header += "\n\t output reg " + output + ",";
    }
    header.pop_back(); // remove last comma
    header += "\n);\n";

    const std::string footer = "endmodule";

    int n = 1;
    for (const auto &node: module->nodes) {
        if (node->GetSerializationType() != "OutputNode")
            continue;
        inner += "\t\t// Output" + std::to_string(n++) + "\n";
        node->accept(*this, 0);
        returnVals.pop();
    }


    std::string out = header + "\n// ─── wire/reg declarations ────────────────────────────────\n" + decls +
                      "\n// ─── combination logic ────────────────────────────────────\n" +

                      "\talways @(*) begin\n" + inner + "\tend\n\n" +
                      "\n// ─── clocked logic ────────────────────────────────────────\n" + later + footer;


    if (std::ofstream file(OUTPUT_PATH); file.is_open()) {
        std::cout << "Writing output file..." << std::endl;
        file << out;
        file.close();
    } else {
        std::cerr << "Could not open file \"" << OUTPUT_PATH << "\"" << std::endl;
    }
}

// ───── MULTI OUTPUT NODES ────────────────────────────────────────────────────────────────────────────────────────────
void Codegen::visit(SplitterNode &node, int output_slot) {
    CHECK_CACHE

    std::vector<std::string> out_wire_names;

    // Cache each output wire
    for (int i = 0; i < node.bits; i++) {
        const std::string bit_output = GetSafeWireName("splitter_b" + std::to_string(i) + "_out");
        out_wire_names.push_back(bit_output);
        visitedNodes[NODE_KEY(i + 1)] = bit_output;
    }

    // Input pin
    const auto in = node.GetInputPin().GetConnectedPin();
    // Verify connection to input pin
    VERIFY_CONNECTION(in);
    // Get input value
    const auto input_val = EvalNode(in);

    // Declare each individual bit as a wire
    decls += "reg ";
    for (int i = 0; i < node.bits; i++) {
        decls += out_wire_names[i] + ", ";
    }
    // Remove extra trailing comma and space
    decls.pop_back();
    decls.pop_back();
    // End line
    decls += ";\n";

    // Assign each wire to single bit
    for (int i = 0; i < node.bits; i++) {
        inner += "\t\t" + out_wire_names[i] + " = " + input_val + "[" + std::to_string(i) + "];\n";
    }

    // Return correct output depending on output slot
    // Skip over the input pin (pin 0)
    const int bit_index = output_slot - 1;
    RETURN_REG(out_wire_names[bit_index]);
}


void Codegen::visit(EdgeNode &node, const int output_slot) {
    CHECK_CACHE

    const std::string output_reg_rise = GetSafeWireName("edge_rise");
    const std::string output_reg_fall = GetSafeWireName("edge_fall");

    visitedNodes[NODE_KEY(node.EDGE_OUT_Q_ID)] = output_reg_rise;
    visitedNodes[NODE_KEY(node.EDGE_OUT_NQ_ID)] = output_reg_fall;

    // Input pins
    const auto d = node.GetDPin().GetConnectedPin();
    const auto clk = node.GetClockPin().GetConnectedPin();

    VERIFY_CONNECTION(d);
    VERIFY_CONNECTION(clk);

    const auto d_val = EvalNode(d);
    const auto clk_val = EvalNode(clk);

    std::cout << "evaluating edge node ix: " << output_slot << std::endl;

    // declare output register and previous register
    const std::string previous_reg = GetSafeWireName("edge_prev");

    decls += "reg " + output_reg_rise + ";\n";
    decls += "reg " + output_reg_fall + ";\n";
    decls += "reg " + previous_reg + ";\n";


    // edge block
    later += "\talways @(posedge " + clk_val + ") begin\n";
    later += "\t\t" + output_reg_rise + " <= " + d_val + " & ~" + previous_reg + ";\n";
    later += "\t\t" + output_reg_fall + " <= ~" + d_val + " & " + previous_reg + ";\n";
    later += "\t\t" + previous_reg + " <= " + d_val + ";\n";
    later += "\tend\n\n";


    // Rising edge option
    if (output_slot == node.EDGE_OUT_Q_ID) {
        RETURN_REG(output_reg_rise);
    }
    // Falling edge option
    if (output_slot == node.EDGE_OUT_NQ_ID) {
        RETURN_REG(output_reg_fall);
    }

    // Fallback - different output node not recognized??
    CircuitError("Invalid connection!", node);
}

// ───── SINGLE OUTPUT NODES ───────────────────────────────────────────────────────────────────────────────────────────
void Codegen::visit(CombinerNode &node, int output_slot) {
    CHECK_CACHE

    // Store evaluated pins
    std::vector<std::string> input_pin_values;

    // Save each input pin value from msb -> lsb
    for (int i = node.bits - 1; i >= 0; i--) {
        // Input pin
        const auto in = node.GetBitInputPin(i).GetConnectedPin();
        // Verify connection to input pin
        VERIFY_CONNECTION(in);
        // Get input value
        const auto input_val = EvalNode(in);

        // Save value
        input_pin_values.push_back(input_val);
    }

    const std::string output_reg = GetSafeWireName("combiner_out");
    // Declare output bus
    decls += "reg [" + std::to_string(node.bits - 1) + ":0] " + output_reg + ";\n";


    // Assignment statement in always
    inner += "\t\t" + output_reg + " = {";
    for (const auto &val: input_pin_values) {
        inner += val + ", ";
    }

    // Remove extra trailing comma and space
    inner.pop_back();
    inner.pop_back();
    // End line
    inner += "};\n";

    RETURN_REG(output_reg);
}


void Codegen::visit(DebounceNode &node, const int output_slot) {
    CHECK_CACHE

    std::string output_reg = GetSafeWireName("debounce_out");
    visitedNodes[NODE_KEY(output_slot)] = output_reg;

    // Input pins
    const auto d = node.GetDPin().GetConnectedPin();
    const auto clk = node.GetClockPin().GetConnectedPin();

    VERIFY_CONNECTION(d);
    VERIFY_CONNECTION(clk);

    const auto d_val = EvalNode(d);
    const auto clk_val = EvalNode(clk);


    // declare output register and shift register
    std::string output_sr = GetSafeWireName("debounce_sr");
    decls += "reg [15:0]" + output_sr + ";\n";
    decls += "reg " + output_reg + ";\n";


    // debounce shift block
    later += "\talways @(posedge " + clk_val + ") begin\n";
    later += "\t\t" + output_sr + " <= { " + output_sr + "[14:0], " + d_val + " };\n";
    later += "\tend\n\n";

    // debounce output register set
    later += "\talways @(posedge " + clk_val + ") begin\n";
    later += "\t\tif (" + output_sr + " == 16'hFFFF)\n";
    later += "\t\t\t" + output_reg + " <= 1'b1;\n";
    later += "\t\telse if (" + output_sr + " == 16'h0000)\n";
    later += "\t\t\t" + output_reg + " <= 1'b0;\n";
    later += "\tend\n\n";


    RETURN_REG(output_reg)
}


void Codegen::visit(RegisterNode &node, int output_slot) {
    CHECK_CACHE

    std::string output_reg = GetSafeWireName("register_value");
    visitedNodes[NODE_KEY(output_slot)] = output_reg;

    // Input pins
    const auto enb = node.GetEnablePin().GetConnectedPin();
    const auto d = node.GetDPin().GetConnectedPin();
    const auto clk = node.GetClkPin().GetConnectedPin();
    const auto rst = node.GetResetPin().GetConnectedPin();

    // Verify connections to pins
    VERIFY_CONNECTION(enb);
    VERIFY_CONNECTION(d);
    VERIFY_CONNECTION(clk);
    VERIFY_CONNECTION(rst);

    // Evaluate inputs
    const auto enb_val = EvalNode(enb);
    const auto d_val = EvalNode(d);
    const auto clk_val = EvalNode(clk);
    const auto rst_val = EvalNode(rst);

    decls += "reg [" + std::to_string(node.bits - 1) + ":0] " + output_reg + ";\n";

    // counter block
    later += "\talways @(posedge " + clk_val + ") begin\n";
    later += "\t\tif (" + rst_val + ") \n";
    later += "\t\t\t" + output_reg + " <= " + std::to_string(node.bits) + "'b0;\n";
    later += "\t\telse if (" + enb_val + " )\n";
    later += "\t\t\t" + output_reg + " <= " + d_val + ";\n";
    later += "\tend\n\n";

    RETURN_REG(output_reg)
}


void Codegen::visit(CounterNode &node, int output_slot) {
    CHECK_CACHE

    std::string output_reg = GetSafeWireName("counter_out");
    visitedNodes[NODE_KEY(output_slot)] = output_reg;

    // Input pins
    const auto enb = node.GetEnablePin().GetConnectedPin();
    const auto cup = node.GetCountUpPin().GetConnectedPin();
    const auto clk = node.GetClkPin().GetConnectedPin();
    const auto rst = node.GetResetPin().GetConnectedPin();

    // Verify connections to pins
    VERIFY_CONNECTION(enb);
    VERIFY_CONNECTION(cup);
    VERIFY_CONNECTION(clk);
    VERIFY_CONNECTION(rst);

    // Evaluate inputs
    const auto enb_val = EvalNode(enb);
    const auto cup_val = EvalNode(cup);
    const auto clk_val = EvalNode(clk);
    const auto rst_val = EvalNode(rst);

    decls += "reg [" + std::to_string(node.bits - 1) + ":0] " + output_reg + ";\n";

    // counter block
    later += "\talways @(posedge " + clk_val + ") begin\n";
    later += "\t\tif (" + rst_val + ") \n";
    later += "\t\t\t" + output_reg + " <= " + std::to_string(node.bits) + "'b0;\n";
    later += "\t\telse if (" + enb_val + " & " + cup_val + " )\n";
    later += "\t\t\t" + output_reg + " <= " + output_reg + " + 1;\n";
    later += "\t\telse if (" + enb_val + " & ~" + cup_val + " )\n";
    later += "\t\t\t" + output_reg + " <= " + output_reg + " - 1;\n";
    later += "\tend\n\n";

    RETURN_REG(output_reg)
}


void Codegen::visit(DFFNode &node, const int output_slot) {
    CHECK_CACHE

    std::string output_reg = GetSafeWireName("dff_out");
    visitedNodes[NODE_KEY(output_slot)] = output_reg;

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

    std::cout << "set pin name: " << set->GetName() << "    slot: " << set->GetNodeIndex() << std::endl;
    const auto set_val = EvalNode(set);
    std::cout << "end: " << std::endl;
    const auto rst_val = EvalNode(rst);
    const auto d_val = EvalNode(d);
    const auto clk_val = EvalNode(clk);

    // declare output register
    decls += "reg " + output_reg + ";\n";

    // Declare clocked logic
    later += "\talways @(posedge " + clk_val + ") begin\n";
    later += "\t\tif (" + rst_val + ")\n";
    later += "\t\t\t" + output_reg + " <= 1'b0;\n"; // TODO add option for reset value to node
    later += "\t\telse if (" + set_val + ")\n";
    later += "\t\t\t" + output_reg + " <= " + d_val + ";\n";
    later += "\tend\n\n";

    RETURN_REG(output_reg);
}


void Codegen::visit(BinaryOpNode &node, const int output_slot) {
    CHECK_CACHE
    START_CHECK_CYCLES

    const auto a = node.GetAInputPin().GetConnectedPin();
    const auto b = node.GetBInputPin().GetConnectedPin();

    VERIFY_CONNECTION(a);
    VERIFY_CONNECTION(b);

    const auto b_val = EvalNode(b);
    const auto a_val = EvalNode(a);


    std::string intermediate_wire_name = GetSafeWireName("bin_op_result");

    decls += "reg " + intermediate_wire_name + ";\n";
    inner += "\t\t" + node.GetVerilogAssign(intermediate_wire_name, a_val, b_val);

    END_CHECK_CYCLES
    CACHE_AND_RETURN(node, intermediate_wire_name, output_slot)
}


void Codegen::visit(UnaryOpNode &node, int output_slot) {
    CHECK_CACHE
    START_CHECK_CYCLES

    const auto a = node.GetAInputPin().GetConnectedPin();

    VERIFY_CONNECTION(a);

    const auto a_val = EvalNode(a);


    std::string intermediate_wire_name = GetSafeWireName("bin_op_result");

    decls += "reg " + intermediate_wire_name + ";\n";
    inner += "\t\t" + node.GetVerilogAssign(intermediate_wire_name, a_val);

    END_CHECK_CYCLES
    CACHE_AND_RETURN(node, intermediate_wire_name, output_slot)
}


void Codegen::visit(MultiplexerNode &node, const int output_slot) {
    CHECK_CACHE
    START_CHECK_CYCLES

    const auto a = node.GetAInputPin().GetConnectedPin();
    const auto b = node.GetBInputPin().GetConnectedPin();
    const auto s = node.GetSelectInputPin().GetConnectedPin();

    VERIFY_CONNECTION(a);
    VERIFY_CONNECTION(b);
    VERIFY_CONNECTION(s);


    const auto select_val = EvalNode(s);
    const auto a_val = EvalNode(a);
    const auto b_val = EvalNode(b);


    std::string intermediate_wire_name = GetSafeWireName("mux_result");
    decls += "reg [" + std::to_string(node.data_bits - 1) + ":0] " + intermediate_wire_name + ";\n";

    inner += "\tif(" + select_val + ")\n\t\t";
    inner += intermediate_wire_name + " = " + a_val + ";\n";
    inner += "\telse\n\t\t";
    inner += intermediate_wire_name + " = " + b_val + ";\n";

    END_CHECK_CYCLES
    CACHE_AND_RETURN(node, intermediate_wire_name, output_slot)
}


void Codegen::visit(ClockNode &node, const int output_slot) {
    CHECK_CACHE
    CACHE_AND_RETURN(node, "sys_clk", output_slot)
}


void Codegen::visit(LiteralNode &node, const int output_slot) {
    CHECK_CACHE
    START_CHECK_CYCLES

    std::string wire_name = GetSafeWireName("number_literal");
    decls += "wire [" + std::to_string(node.bits - 1) + ":0] " + wire_name + " = " + std::to_string(node.bits) + "'d" +
             std::to_string(node.value) + ";\n";

    END_CHECK_CYCLES
    CACHE_AND_RETURN(node, wire_name, output_slot)
}


void Codegen::visit(InputNode &node, const int output_slot) {
    CHECK_CACHE
    START_CHECK_CYCLES

    END_CHECK_CYCLES
    CACHE_AND_RETURN(node, node.module->inputs[node.slot], output_slot)
}


void Codegen::visit(OutputNode &node, const int output_slot) {
    CHECK_CACHE
    START_CHECK_CYCLES

    const auto in = node.GetValueInputPin().GetConnectedPin();
    VERIFY_CONNECTION(in);

    const auto in_val = EvalNode(in);

    inner += "\t\t" + node.module->outputs[node.slot] + " = " + in_val + ";\n";

    END_CHECK_CYCLES
    CACHE_AND_RETURN(node, "", output_slot);
}


std::optional<std::string> Codegen::CheckCache(const std::string &guid) {
    if (!visitedNodes.contains(guid))
        return std::nullopt;

    return visitedNodes[guid];
}

std::string Codegen::GetSafeWireName(const std::string &wire_name) {
    if (wireNameCounts.contains(wire_name)) {
        return wire_name + std::to_string(wireNameCounts[wire_name]++);
    }

    wireNameCounts[wire_name] = 1;
    return wire_name + "0";
}

void Codegen::CircuitError(const std::string &msg, const Node &node) const {
    std::cerr << "ERROR EXPORTING CIRCUIT: " << msg << std::endl;
    std::cerr << "related node: " << node.guid << std::endl;
    error_manager->ThrowError(msg, node);
}

bool Codegen::CheckActive(const std::string &guid) {
    std::stack<std::string> tmp;
    bool found = false;

    while (!activeNodes.empty()) {
        if (activeNodes.top() == guid) {
            found = true;
            break;
        }
        tmp.push(activeNodes.top());
        activeNodes.pop();
    }

    while (!tmp.empty()) {
        activeNodes.push(tmp.top());
        tmp.pop();
    }
    return found;
}
