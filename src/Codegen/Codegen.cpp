//
// Created by gabed on 4/15/2026.
//

#include "Codegen.h"

#include <fstream>


#include "Default/AdderNode.h"
#include "Default/BinaryOperator/OrNode.h"
#include "Default/ClockNode.h"
#include "Default/CombinerNode.h"
#include "Default/CounterNode.h"
#include "Default/CustomModuleNode.h"
#include "Default/DFFNode.h"
#include "Default/DebounceNode.h"
#include "Default/DecoderNode.h"
#include "Default/EdgeNode.h"
#include "Default/InputNode.h"
#include "Default/LiteralNode.h"
#include "Default/MultiplexerNode.h"
#include "Default/OutputNode.h"
#include "Default/RegisterNode.h"
#include "Default/SplitterNode.h"
#include "Default/SubtractorNode.h"
#include "Default/UnaryOperator/UnaryOpNode.h"
#include "Module.h"

#define OUTPUT_DIR "../Project/Export/"


#define NODE_KEY(n) (node.guid + "-slot-" + std::to_string(n))


#define CHECK_CACHE                                                                                                    \
    {                                                                                                                  \
        auto memo = CheckCache(NODE_KEY(output_slot));                                                                 \
        if (memo.has_value()) {                                                                                        \
            m_return_vals.emplace(memo.value());                                                                       \
            std::cout << "used cache: " << node.name << std::endl;                                                     \
            return;                                                                                                    \
        }                                                                                                              \
    }
#define CACHE_AND_RETURN(node, val, slot)                                                                              \
    {                                                                                                                  \
        m_visited_nodes[NODE_KEY(slot)] = val;                                                                         \
        m_return_vals.emplace(val);                                                                                    \
        return;                                                                                                        \
    }


#define RETURN_REG(val)                                                                                                \
    {                                                                                                                  \
        m_return_vals.emplace(val);                                                                                    \
        return;                                                                                                        \
    }

#define ERROR_AND_RETURN                                                                                               \
    {                                                                                                                  \
        m_visited_nodes[NODE_KEY(output_slot)] = "err";                                                                \
        m_return_vals.emplace("err");                                                                                  \
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
        m_active_nodes.emplace(node.guid);                                                                             \
    }
#define END_CHECK_CYCLES m_active_nodes.pop();


Codegen::Codegen(std::shared_ptr<ErrorManager> error_man) : m_failed(false), m_error_manager(std::move(error_man)) {}

void Codegen::GenerateCode(const std::shared_ptr<Module> &module) {
    m_failed = false;
    std::string header = "module " + module->GetName() + " (";
    for (const auto &[name, bits]: module->GetInputs()) {
        if (bits == 1)
            header += "\n\tinput wire " + name + ",";
        else
            header += "\n\tinput wire [" + std::to_string(bits - 1) + ":0] " + name + ",";
    }
    header += "\n\tinput wire sys_clk,";
    for (const auto &[name, bits]: module->GetOutputs()) {
        if (bits == 1)
            header += "\n\toutput reg " + name + ",";
        else
            header += "\n\toutput reg [" + std::to_string(bits - 1) + ":0] " + name + ",";
    }
    header.pop_back(); // remove last comma
    header += "\n);\n";

    const std::string footer = "endmodule";

    for (const auto &node: module->GetNodes()) {
        if (node->GetSerializationType() != "OutputNode")
            continue;
        m_inner += "\t\t// Output " + module->GetOutputs()[dynamic_cast<OutputNode *>(node.get())->slot].name + "\n";
        node->accept(*this, 0);
        m_return_vals.pop();
    }


    const std::string out = header + "\n// === wire/reg declarations ================================\n" + m_decls +
                            "\n// === module instances =====================================\n" + m_instances +
                            "\n// === combination logic ====================================\n" +

                            "\talways @(*) begin\n" + m_inner + "\tend\n\n" +
                            "\n// === clocked logic ========================================\n" + m_later + footer;

    const std::string out_path = OUTPUT_DIR + module->GetName() + ".v";

    if (std::ofstream file(out_path); file.is_open()) {
        std::cout << "Writing output file..." << std::endl;
        file << out;
        file.close();
    } else {
        std::cerr << "Could not open file \"" << out_path << "\"" << std::endl;
    }
}

// ===== MULTI OUTPUT NODES ============================================================================================
void Codegen::visit(CustomModuleNode &node, const int output_slot) {
    CHECK_CACHE
    START_CHECK_CYCLES

    if (!node.module_ref.has_value()) {
        CircuitError("Refrence to missing module in custom node!", node);
        ERROR_AND_RETURN
    }

    const auto &node_module = node.module_ref.value();
    const int num_inputs = static_cast<int>(node_module->GetInputs().size());
    const int num_outputs = static_cast<int>(node_module->GetOutputs().size());

    std::vector<IO> output_wires;
    std::vector<std::string> output_net_name;
    // Cache each output wires name
    for (int i = 0; i < num_outputs; i++) {
        // Input pin
        const int pin_number = num_inputs + i;
        const auto output_name =
                GetSafeWireName(node_module->GetName() + "_custom_out_" + node.pins[pin_number].GetName());

        output_wires.push_back({output_name, node.pins[pin_number].GetDataType().GetBitWidth()});
        output_net_name.push_back(node.pins[pin_number].GetName());
        m_visited_nodes[NODE_KEY(pin_number)] = output_name;
    }


    // const auto save = m_active_nodes;
    // m_active_nodes = {};

    std::vector<std::string> input_pin_values;
    std::vector<std::string> input_pin_names;

    // Evaluate and cache all inputs
    for (int i = 0; i < num_inputs; i++) {
        // Input pin
        const auto in = node.pins[i].GetConnectedPin();
        // Verify connection to input pin
        VERIFY_CONNECTION(in);
        // Get input value
        const auto input_val = EvalNode(in);

        // Save value
        input_pin_values.push_back(input_val);
        input_pin_names.push_back(node.pins[i].GetName());
    }

    // m_active_nodes = save;

    // Create output wires
    for (const auto &[name, bits]: output_wires) {
        m_decls += "wire [" + std::to_string(bits - 1) + ":0] " + name + ";\n";
    }

    // Instantiate module and connect labeled inputs
    m_instances += node_module->GetName() + " " + GetSafeWireName("custom_node") + " (\n";
    m_instances += "\t.sys_clk(sys_clk),\n";
    for (int i = 0; i < num_inputs; i++) {
        m_instances += "\t." + input_pin_names[i] + "(" + input_pin_values[i] + "),\n";
    }
    for (int i = 0; i < num_outputs; i++) {
        m_instances += "\t." + output_net_name[i] + "(" + output_wires[i].name + "),\n";
    }
    m_instances.pop_back();
    m_instances.pop_back();
    m_instances += "\n);\n";

    RETURN_REG(output_wires[output_slot - num_inputs].name);

    END_CHECK_CYCLES
    //  TODO retunnr??
}

void Codegen::visit(SplitterNode &node, const int output_slot) {
    CHECK_CACHE
    START_CHECK_CYCLES
    std::vector<std::string> out_wire_names;

    // Cache each output wire
    for (int i = 0; i < node.GetDataWidth(); i++) {
        const std::string bit_output = GetSafeWireName("splitter_b" + std::to_string(i) + "_out");
        out_wire_names.push_back(bit_output);
        m_visited_nodes[NODE_KEY(i + 1)] = bit_output;
    }

    // Input pin
    const auto in = node.GetInputPin().GetConnectedPin();
    // Verify connection to input pin
    VERIFY_CONNECTION(in);
    // Get input value
    const auto input_val = EvalNode(in);

    // Declare each individual bit as a wire
    m_decls += "reg ";
    for (int i = 0; i < node.GetDataWidth(); i++) {
        m_decls += out_wire_names[i] + ", ";
    }
    // Remove extra trailing comma and space
    m_decls.pop_back();
    m_decls.pop_back();
    // End line
    m_decls += ";\n";

    // Assign each wire to single bit
    for (int i = 0; i < node.GetDataWidth(); i++) {
        m_inner += "\t\t" + out_wire_names[i] + " = " + input_val + "[" + std::to_string(i) + "];\n";
    }

    // Return correct output depending on output slot
    // Skip over the input pin (pin 0)
    const int bit_index = output_slot - 1;
    END_CHECK_CYCLES
    RETURN_REG(out_wire_names[bit_index]);
}

void Codegen::visit(EdgeNode &node, const int output_slot) {
    CHECK_CACHE

    const std::string output_reg_rise = GetSafeWireName("edge_rise");
    const std::string output_reg_fall = GetSafeWireName("edge_fall");

    m_visited_nodes[NODE_KEY(node.EDGE_OUT_Q_ID)] = output_reg_rise;
    m_visited_nodes[NODE_KEY(node.EDGE_OUT_NQ_ID)] = output_reg_fall;

    // Input pins
    const auto d = node.GetDPin().GetConnectedPin();
    const auto clk = node.GetClockPin().GetConnectedPin();

    VERIFY_CONNECTION(d);
    VERIFY_CONNECTION(clk);

    const auto d_val = EvalNode(d);
    const auto clk_val = EvalNode(clk);

    // declare output register and previous register
    const std::string previous_reg = GetSafeWireName("edge_prev");

    m_decls += "reg " + output_reg_rise + ";\n";
    m_decls += "reg " + output_reg_fall + ";\n";
    m_decls += "reg " + previous_reg + ";\n";


    // edge block
    m_later += "\talways @(posedge " + clk_val + ") begin\n";
    m_later += "\t\t" + output_reg_rise + " <= " + d_val + " & ~" + previous_reg + ";\n";
    m_later += "\t\t" + output_reg_fall + " <= ~" + d_val + " & " + previous_reg + ";\n";
    m_later += "\t\t" + previous_reg + " <= " + d_val + ";\n";
    m_later += "\tend\n\n";


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

void Codegen::visit(AdderNode &node, const int output_slot) {
    CHECK_CACHE

    const std::string output_value = GetSafeWireName("adder_out");
    const std::string output_carry = GetSafeWireName("adder_carry_out");

    m_visited_nodes[NODE_KEY(node.ADDER_Q_ID)] = output_value;
    m_visited_nodes[NODE_KEY(node.ADDER_COUT_ID)] = output_carry;

    // Input pins
    const auto a = node.GetAInputPin().GetConnectedPin();
    const auto b = node.GetBInputPin().GetConnectedPin();
    const auto cin = node.GetCarryInputPin().GetConnectedPin();

    VERIFY_CONNECTION(a);
    VERIFY_CONNECTION(b);
    VERIFY_CONNECTION(cin);

    const auto a_val = EvalNode(a);
    const auto b_val = EvalNode(b);
    const auto cin_val = EvalNode(cin);

    m_decls += "reg [" + std::to_string(node.GetDataWidth() - 1) + ":0] " + output_value + ";\n";
    m_decls += "reg " + output_carry + ";\n";

    m_inner += "\t\t{" + output_carry + ", " + output_value + "} = " + a_val + " + " + b_val + " + " + cin_val + ";\n";

    // Rising edge option
    if (output_slot == node.ADDER_Q_ID) {
        RETURN_REG(output_value);
    }
    // Falling edge option
    if (output_slot == node.ADDER_COUT_ID) {
        RETURN_REG(output_carry);
    }

    // Fallback - different output node not recognized??
    CircuitError("Invalid connection!", node);
}

void Codegen::visit(SubtractorNode &node, int output_slot) {
    CHECK_CACHE

    const std::string output_value = GetSafeWireName("sub_out");
    const std::string output_carry = GetSafeWireName("sub_carry_out");

    m_visited_nodes[NODE_KEY(node.SUBTRACTOR_Q_ID)] = output_value;
    m_visited_nodes[NODE_KEY(node.SUBTRACTOR_COUT_ID)] = output_carry;

    // Input pins
    const auto a = node.GetAInputPin().GetConnectedPin();
    const auto b = node.GetBInputPin().GetConnectedPin();

    VERIFY_CONNECTION(a);
    VERIFY_CONNECTION(b);

    const auto a_val = EvalNode(a);
    const auto b_val = EvalNode(b);

    m_decls += "reg [" + std::to_string(node.GetDataWidth() - 1) + ":0] " + output_value + ";\n";
    m_decls += "reg " + output_carry + ";\n";

    // Subtraction with extension first
    m_inner += "\t\t{" + output_carry + ", " + output_value + "} = {1'b0, " + a_val + "} - {1'b0, " + b_val + "};\n";

    // Rising edge option
    if (output_slot == node.SUBTRACTOR_Q_ID) {
        RETURN_REG(output_value);
    }
    // Falling edge option
    if (output_slot == node.SUBTRACTOR_COUT_ID) {
        RETURN_REG(output_carry);
    }

    // Fallback - different output node not recognized??
    CircuitError("Invalid connection!", node);
}

void Codegen::visit(DecoderNode &node, const int output_slot) {
    CHECK_CACHE
    START_CHECK_CYCLES
    std::vector<std::string> out_wire_names;

    // Cache each output wire
    for (int i = 0; i < node.GetNumOutputs(); i++) {
        const std::string bit_output = GetSafeWireName("decoder_" + std::to_string(i) + "_out");
        out_wire_names.push_back(bit_output);
        m_visited_nodes[NODE_KEY(i + 1)] = bit_output;
    }

    // Input pin
    const auto in = node.GetInputPin().GetConnectedPin();
    // Verify connection to input pin
    VERIFY_CONNECTION(in);
    // Get input value
    const auto input_val = EvalNode(in);

    // Declare each individual bit as a wire
    m_decls += "reg ";
    for (int i = 0; i < node.GetNumOutputs(); i++) {
        m_decls += out_wire_names[i] + ", ";
    }
    // Remove extra trailing comma and space
    m_decls.pop_back();
    m_decls.pop_back();
    // End line
    m_decls += ";\n";

    // Initalize all outputs to 0
    m_inner += "\t\t{";
    // Assign each wire to single bit
    for (int i = 0; i < node.GetNumOutputs(); i++) {
        m_inner += out_wire_names[i] + ", ";
    }
    m_inner.pop_back();
    m_inner.pop_back();
    m_inner += "} = " + std::to_string(node.GetNumOutputs()) + "'b0;\n";

    m_inner += "\t\tcase (" + input_val + ")\n";
    for (int i = 0; i < node.GetNumOutputs(); i++) {
        m_inner += "\t\t\t" + std::to_string(node.GetDataWidth()) + "'d" + std::to_string(i) + ": " +
                   out_wire_names[i] + " = 1;\n";
    }
    m_inner += "\t\tendcase\n";


    // Return correct output depending on output slot
    // Skip over the input pin (pin 0)
    const int bit_index = output_slot - 1;
    END_CHECK_CYCLES
    RETURN_REG(out_wire_names[bit_index]);
}

// ===== SINGLE OUTPUT NODES ===========================================================================================
void Codegen::visit(CombinerNode &node, const int output_slot) {
    CHECK_CACHE
    START_CHECK_CYCLES
    // Store evaluated pins
    std::vector<std::string> input_pin_values;

    // Save each input pin value from msb -> lsb
    for (int i = node.GetDataWidth() - 1; i >= 0; i--) {
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
    m_decls += "reg [" + std::to_string(node.GetDataWidth() - 1) + ":0] " + output_reg + ";\n";


    // Assignment statement in always
    m_inner += "\t\t" + output_reg + " = {";
    for (const auto &val: input_pin_values) {
        m_inner += val + ", ";
    }

    // Remove extra trailing comma and space
    m_inner.pop_back();
    m_inner.pop_back();
    // End line
    m_inner += "};\n";
    END_CHECK_CYCLES
    CACHE_AND_RETURN(node, output_reg, output_slot)
}


void Codegen::visit(MultiplexerNode &node, const int output_slot) {
    CHECK_CACHE
    START_CHECK_CYCLES

    std::string result_reg = GetSafeWireName("mux_result");
    m_visited_nodes[NODE_KEY(output_slot)] = result_reg;

    // Store evaluated pins
    std::vector<std::string> input_pin_values;

    // Save each mux imput pin
    for (int i = 0; i < node.GetNumOptions(); i++) {
        std::cout << "eval mux pin " << i << std::endl;
        // Input pin
        const auto in = node.GetInputPin(i).GetConnectedPin();
        // Verify connection to input pin
        VERIFY_CONNECTION(in);
        // Get input value
        const auto input_val = EvalNode(in);

        std::cout << " got " << input_val << std::endl;

        // Save value
        input_pin_values.push_back(input_val);
    }

    const auto s = node.GetSelectInputPin().GetConnectedPin();
    VERIFY_CONNECTION(s);

    const auto select_val = EvalNode(s);


    m_decls += "reg [" + std::to_string(node.GetDataWidth() - 1) + ":0] " + result_reg + ";\n";


    m_inner += "\t\tcase (" + select_val + ")\n";
    for (int i = 0; i < node.GetNumOptions(); i++) {
        m_inner += "\t\t\t" + std::to_string(node.GetSelectWidth()) + "'d" + std::to_string(i) + ": " + result_reg +
                   " = " + input_pin_values[i] + ";\n";
    }
    m_inner += "\t\tendcase\n\n";


    END_CHECK_CYCLES
    CACHE_AND_RETURN(node, result_reg, output_slot)
}


void Codegen::visit(DebounceNode &node, const int output_slot) {
    CHECK_CACHE

    std::string output_reg = GetSafeWireName("debounce_out");
    m_visited_nodes[NODE_KEY(output_slot)] = output_reg;

    // Input pins
    const auto d = node.GetDPin().GetConnectedPin();
    const auto clk = node.GetClockPin().GetConnectedPin();

    VERIFY_CONNECTION(d);
    VERIFY_CONNECTION(clk);

    const auto d_val = EvalNode(d);
    const auto clk_val = EvalNode(clk);


    // declare output register and shift register
    const std::string output_sr = GetSafeWireName("debounce_sr");
    m_decls += "reg [15:0]" + output_sr + ";\n";
    m_decls += "reg " + output_reg + ";\n";


    // debounce shift block
    m_later += "\talways @(posedge " + clk_val + ") begin\n";
    m_later += "\t\t" + output_sr + " <= { " + output_sr + "[14:0], " + d_val + " };\n";
    m_later += "\tend\n\n";

    // debounce output register set
    m_later += "\talways @(posedge " + clk_val + ") begin\n";
    m_later += "\t\tif (" + output_sr + " == 16'hFFFF)\n";
    m_later += "\t\t\t" + output_reg + " <= 1'b1;\n";
    m_later += "\t\telse if (" + output_sr + " == 16'h0000)\n";
    m_later += "\t\t\t" + output_reg + " <= 1'b0;\n";
    m_later += "\tend\n\n";


    RETURN_REG(output_reg)
}


void Codegen::visit(RegisterNode &node, const int output_slot) {
    CHECK_CACHE

    std::string output_reg = GetSafeWireName("register_value");
    m_visited_nodes[NODE_KEY(output_slot)] = output_reg;

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

    m_decls += "reg [" + std::to_string(node.GetDataWidth() - 1) + ":0] " + output_reg + ";\n";

    // counter block
    m_later += "\talways @(posedge " + clk_val + ") begin\n";
    m_later += "\t\tif (" + rst_val + ") \n";
    m_later += "\t\t\t" + output_reg + " <= " + std::to_string(node.GetDataWidth()) + "'b0;\n";
    m_later += "\t\telse if (" + enb_val + " )\n";
    m_later += "\t\t\t" + output_reg + " <= " + d_val + ";\n";
    m_later += "\tend\n\n";

    RETURN_REG(output_reg)
}


void Codegen::visit(CounterNode &node, const int output_slot) {
    CHECK_CACHE

    std::string output_reg = GetSafeWireName("counter_out");
    m_visited_nodes[NODE_KEY(output_slot)] = output_reg;

    // Input pins
    const auto enb = node.GetEnablePin().GetConnectedPin();
    const auto cup = node.GetCountUpPin().GetConnectedPin();
    const auto clk = node.GetClkPin().GetConnectedPin();
    const auto rst = node.GetResetPin().GetConnectedPin();

    const auto load_enb = node.GetLoadPin().GetConnectedPin();
    const auto load_val = node.GetLoadValuePin().GetConnectedPin();

    // Verify connections to pins
    VERIFY_CONNECTION(enb);
    VERIFY_CONNECTION(cup);
    VERIFY_CONNECTION(clk);
    VERIFY_CONNECTION(rst);
    VERIFY_CONNECTION(load_enb);
    VERIFY_CONNECTION(load_val);

    // Evaluate inputs
    const auto enb_val = EvalNode(enb);
    const auto cup_val = EvalNode(cup);
    const auto clk_val = EvalNode(clk);
    const auto rst_val = EvalNode(rst);
    const auto load_enb_val = EvalNode(load_enb);
    const auto load_val_val = EvalNode(load_val);

    m_decls += "reg [" + std::to_string(node.GetDataWidth() - 1) + ":0] " + output_reg + ";\n";

    // counter block
    m_later += "\talways @(posedge " + clk_val + ") begin\n";
    m_later += "\t\tif (" + rst_val + ") begin \n";
    m_later += "\t\t\t" + output_reg + " <= " + std::to_string(node.GetDataWidth()) + "'b0;\n";
    m_later += "\t\tend else if (" + load_enb_val + " ) begin\n";
    m_later += "\t\t\t" + output_reg + " <= " + load_val_val + ";\n";
    m_later += "\t\tend else if (" + enb_val + " & " + cup_val + " ) begin\n";
    m_later += "\t\t\t" + output_reg + " <= " + output_reg + " + 1;\n";
    m_later += "\t\tend else if (" + enb_val + " & ~" + cup_val + " ) begin\n";
    m_later += "\t\t\t" + output_reg + " <= " + output_reg + " - 1;\n";
    m_later += "\t\tend\n";
    m_later += "\tend\n\n";

    RETURN_REG(output_reg)
}


void Codegen::visit(DFFNode &node, const int output_slot) {
    CHECK_CACHE

    std::string output_reg = GetSafeWireName("dff_out");
    m_visited_nodes[NODE_KEY(output_slot)] = output_reg;

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

    const auto set_val = EvalNode(set);
    const auto rst_val = EvalNode(rst);
    const auto d_val = EvalNode(d);
    const auto clk_val = EvalNode(clk);

    // declare output register
    m_decls += "reg " + output_reg + ";\n";

    // Declare clocked logic
    m_later += "\talways @(posedge " + clk_val + ") begin\n";
    m_later += "\t\tif (" + rst_val + ")\n";
    m_later += "\t\t\t" + output_reg + " <= 1'b0;\n"; // TODO add option for reset value to node
    m_later += "\t\telse if (" + set_val + ")\n";
    m_later += "\t\t\t" + output_reg + " <= " + d_val + ";\n";
    m_later += "\tend\n\n";

    RETURN_REG(output_reg);
}


void Codegen::visit(BinaryOpNode &node, const int output_slot) {
    CHECK_CACHE
    START_CHECK_CYCLES

    std::vector<std::string> input_pin_values;

    // Save each input pin value
    for (int i = 0; i < node.GetNumInputs(); i++) {
        // Input pin
        const auto in = node.GetInputPin(i).GetConnectedPin();
        // Verify connection to input pin
        VERIFY_CONNECTION(in);
        // Get input value
        const auto input_val = EvalNode(in);

        // Save value
        input_pin_values.push_back(input_val);
    }


    const std::string out_reg = GetSafeWireName("bin_op_result");

    m_decls += "reg [" + std::to_string(node.GetDataWidth() - 1) + ":0] " + out_reg + ";\n";
    m_inner += "\t\t" + node.GetVerilogAssign(out_reg, input_pin_values);

    END_CHECK_CYCLES
    CACHE_AND_RETURN(node, out_reg, output_slot)
}


void Codegen::visit(UnaryOpNode &node, const int output_slot) {
    CHECK_CACHE
    START_CHECK_CYCLES

    const auto a = node.GetAInputPin().GetConnectedPin();

    VERIFY_CONNECTION(a);

    const auto a_val = EvalNode(a);


    const std::string out_reg = GetSafeWireName("bin_op_result");

    m_decls += "reg [" + std::to_string(node.GetDataWidth() - 1) + ":0] " + out_reg + ";\n";
    m_inner += "\t\t" + node.GetVerilogAssign(out_reg, a_val);

    END_CHECK_CYCLES
    CACHE_AND_RETURN(node, out_reg, output_slot)
}


void Codegen::visit(ClockNode &node, const int output_slot) {
    CHECK_CACHE
    CACHE_AND_RETURN(node, "sys_clk", output_slot)
}


void Codegen::visit(LiteralNode &node, const int output_slot) {
    CHECK_CACHE
    START_CHECK_CYCLES

    std::string wire_name = GetSafeWireName("number_literal");
    m_decls += "wire [" + std::to_string(node.GetDataWidth() - 1) + ":0] " + wire_name + " = " +
               std::to_string(node.GetDataWidth()) + "'d" + std::to_string(node.value) + ";\n";

    END_CHECK_CYCLES
    CACHE_AND_RETURN(node, wire_name, output_slot)
}


void Codegen::visit(InputNode &node, const int output_slot) {
    CHECK_CACHE
    START_CHECK_CYCLES

    END_CHECK_CYCLES
    CACHE_AND_RETURN(node, node.module->GetInputs()[node.slot].name, output_slot)
}


void Codegen::visit(OutputNode &node, const int output_slot) {
    CHECK_CACHE
    START_CHECK_CYCLES

    const auto in = node.GetValueInputPin().GetConnectedPin();
    VERIFY_CONNECTION(in);

    const auto in_val = EvalNode(in);

    m_inner += "\t\t" + node.module->GetOutputs()[node.slot].name + " = " + in_val + ";\n";

    END_CHECK_CYCLES
    CACHE_AND_RETURN(node, "", output_slot);
}


std::optional<std::string> Codegen::CheckCache(const std::string &guid) {
    if (!m_visited_nodes.contains(guid))
        return std::nullopt;

    return m_visited_nodes[guid];
}

std::string Codegen::GetSafeWireName(const std::string &wire_name) {
    if (m_wire_name_counts.contains(wire_name)) {
        return wire_name + std::to_string(m_wire_name_counts[wire_name]++);
    }

    m_wire_name_counts[wire_name] = 1;
    return wire_name + "0";
}

void Codegen::CircuitError(const std::string &msg, const Node &node) {
    m_failed = true;
    std::cerr << "ERROR EXPORTING CIRCUIT: " << msg << std::endl;
    std::cerr << "related node: " << node.guid << std::endl;
    m_error_manager->ThrowError(msg, node);
}

bool Codegen::CheckActive(const std::string &guid) {
    std::stack<std::string> tmp;
    bool found = false;

    while (!m_active_nodes.empty()) {
        if (m_active_nodes.top() == guid) {
            std::cout << "Currently evaluating node " << guid << " oh no   " << std::endl;
            found = true;
            break;
        }
        tmp.push(m_active_nodes.top());
        m_active_nodes.pop();
    }

    while (!tmp.empty()) {
        m_active_nodes.push(tmp.top());
        tmp.pop();
    }
    return found;
}
