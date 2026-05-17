//
// Created by Gabe on 5/14/2026.
//
#include "ConstExprEvaluator.h"

#include <iostream>

#include "Default/AdderNode.h"
#include "Default/BinaryOperator/BinaryOpNode.h"
#include "Default/BitSelectorNode.h"
#include "Default/CombinerNode.h"
#include "Default/ComparatorNode.h"
#include "Default/CounterNode.h"
#include "Default/CustomModuleNode.h"
#include "Default/DecoderNode.h"
#include "Default/LiteralNode.h"
#include "Default/MultiplexerNode.h"
#include "Default/MultiplierNode.h"
#include "Default/OutputNode.h"
#include "Default/RAMNode.h"
#include "Default/ROMNode.h"
#include "Default/RegisterNode.h"
#include "Default/ShifterNode.h"
#include "Default/SplitterNode.h"
#include "Default/SubtractorNode.h"
#include "Default/UnaryOperator/UnaryOpNode.h"
#include "Module.h"


#define NODE_KEY(n) (node.guid + "-slot-" + std::to_string(n))

#define MASK(n) ((1u << n) - 1u)

#define CHECK_CACHE                                                                                                    \
    {                                                                                                                  \
        auto memo = CheckCache(NODE_KEY(output_slot));                                                                 \
        if (memo.has_value()) {                                                                                        \
            m_return_vals.push(memo.value());                                                                          \
            std::cout << "used cache: " << node.name << std::endl;                                                     \
            return;                                                                                                    \
        }                                                                                                              \
    }
#define CACHE_AND_RETURN(node, val, slot)                                                                              \
    {                                                                                                                  \
        m_visited_nodes[NODE_KEY(slot)] = val;                                                                         \
        m_return_vals.push(val);                                                                                       \
        return;                                                                                                        \
    }


#define RETURN_SPACE(val)                                                                                              \
    {                                                                                                                  \
        m_return_vals.push(val);                                                                                       \
        return;                                                                                                        \
    }

#define ERROR_AND_RETURN                                                                                               \
    {                                                                                                                  \
        m_visited_nodes[NODE_KEY(output_slot)] = ResultSpace(0);                                                       \
        m_return_vals.push(ResultSpace(0));                                                                            \
        return;                                                                                                        \
    }

#define VERIFY_CONNECTION(PIN)                                                                                         \
    {                                                                                                                  \
        if (!PIN) {                                                                                                    \
            CircuitError("Missing connection to input " #PIN "!", node);                                               \
            ERROR_AND_RETURN                                                                                           \
        }                                                                                                              \
    }


ConstExprEvaluator::ConstExprEvaluator(std::shared_ptr<ErrorManager> error_man) :
    m_error_manager(std::move(error_man)) {}

ResultSpace ConstExprEvaluator::FindSpace(const std::shared_ptr<Node> &node, const int output_slot) {
    node->accept(*this, output_slot);
    const ResultSpace s = m_return_vals.top();
    m_return_vals.pop();
    return s;
}


// =================  TODO modules ==============


void ConstExprEvaluator::visit(ShifterNode &node, const int output_slot) {
    RETURN_SPACE(ResultSpace(node.GetDataWidth()))
}


void ConstExprEvaluator::visit(MultiplierNode &node, const int output_slot) {
    RETURN_SPACE(ResultSpace(node.GetDataWidth()))
}
void ConstExprEvaluator::visit(SubtractorNode &node, const int output_slot) {
    RETURN_SPACE(ResultSpace(node.GetDataWidth()))
}


void ConstExprEvaluator::visit(UnaryOpNode &node, const int output_slot) {
    RETURN_SPACE(ResultSpace(node.GetDataWidth()))
}


// =================== Done

void ConstExprEvaluator::visit(ComparatorNode &node, const int output_slot) {
    const auto a = node.GetAInputPin().GetConnectedPin();
    const auto b = node.GetBInputPin().GetConnectedPin();

    VERIFY_CONNECTION(a)
    VERIFY_CONNECTION(b)

    const auto a_space = EvalNode(a);
    const auto b_space = EvalNode(b);

    if (a_space.IsConstant() && b_space.IsConstant()) {
        const int a_val = a_space.GetConstantValue();
        const int b_val = b_space.GetConstantValue();

        const bool less_than = a_val < b_val;
        const bool equal_to = a_val == b_val;
        const bool greater_than = a_val > b_val;

        if (output_slot == node.COMPARATOR_L_ID)
            RETURN_SPACE(ResultSpace(1, less_than));
        if (output_slot == node.COMPARATOR_E_ID)
            RETURN_SPACE(ResultSpace(1, equal_to));
        if (output_slot == node.COMPARATOR_G_ID)
            RETURN_SPACE(ResultSpace(1, greater_than));
    }

    // All 3 outputs are booleans: 0=false, 1=true
    RETURN_SPACE(ResultSpace(1))
}


// ========== Storage ==================================================================================================
void ConstExprEvaluator::visit(CounterNode &node, const int output_slot) {
    RETURN_SPACE(ResultSpace(node.GetDataWidth()))
}
void ConstExprEvaluator::visit(RegisterNode &node, const int output_slot) {
    RETURN_SPACE(ResultSpace(node.GetDataWidth()))
}
void ConstExprEvaluator::visit(RAMNode &node, const int output_slot) {
    // TODO check if address is constant
    RETURN_SPACE(ResultSpace(node.GetDataWidth()))
}
void ConstExprEvaluator::visit(ROMNode &node, const int output_slot) {
    // TODO check if address is constant
    RETURN_SPACE(ResultSpace(node.GetDataWidth()))
}
void ConstExprEvaluator::visit(DFFNode &node, const int output_slot) { RETURN_SPACE(ResultSpace(1)) }

// ========== Bitwise Manipulation =====================================================================================
void ConstExprEvaluator::visit(BitSelectorNode &node, const int output_slot) {
    const auto in = node.GetInputPin().GetConnectedPin();

    VERIFY_CONNECTION(in)

    const auto in_space = EvalNode(in);

    if (!in_space.IsConstant()) {
        RETURN_SPACE(ResultSpace(node.GetDataRangeWidth()))
    }


    // # of bits to shift over
    const uint32_t start_bit = node.GetStartBit();
    // # of bits to mask after shifting
    const uint32_t out_width = node.GetDataRangeWidth();
    const uint32_t mask = MASK(out_width);

    const uint32_t shifted = in_space.GetConstantValue() >> start_bit;
    const uint32_t done = shifted & mask;

    RETURN_SPACE(ResultSpace(node.GetDataRangeWidth(), done))
}
void ConstExprEvaluator::visit(DecoderNode &node, const int output_slot) {
    const auto input = node.GetInputPin().GetConnectedPin();

    VERIFY_CONNECTION(input)

    const auto input_space = EvalNode(input);
    if (!input_space.IsConstant()) {
        // Each output of decoder is either active or inactive
        RETURN_SPACE(ResultSpace(1))
    }

    const int selected_in = input_space.GetConstantValue();
    const int curr_in = output_slot - 1;

    RETURN_SPACE(ResultSpace(1, selected_in == curr_in ? 1 : 0));
}
void ConstExprEvaluator::visit(BinaryOpNode &node, const int output_slot) {
    std::vector<ResultSpace> input_spaces;
    input_spaces.reserve(node.GetNumInputs());

    bool all_constant = true;

    // Save each input pin value
    for (int i = 0; i < node.GetNumInputs(); i++) {
        // Input pin
        const auto in = node.GetInputPin(i).GetConnectedPin();
        // Verify connection to input pin
        VERIFY_CONNECTION(in)
        // Get input value
        const auto input_space = EvalNode(in);

        // Save value
        input_spaces.push_back(input_space);

        if (!input_space.IsConstant()) {
            all_constant = false;
        }
    }

    if (input_spaces.empty()) {
        CircuitError("BinaryOpNode has no inputs", node);
        ERROR_AND_RETURN;
    }

    if (!all_constant) {
        RETURN_SPACE(ResultSpace(node.GetDataWidth()))
    }

    uint32_t val = input_spaces[0].GetConstantValue();
    const int mask = MASK(node.GetDataWidth());

    if (node.GetSerializationType() == "AndNode") {
        for (int i = 1; i < input_spaces.size(); i++) {
            val &= input_spaces[i].GetConstantValue();
        }
        RETURN_SPACE(ResultSpace(node.GetDataWidth(), val & mask));
    }
    if (node.GetSerializationType() == "OrNode") {
        for (int i = 1; i < input_spaces.size(); i++) {
            val |= input_spaces[i].GetConstantValue();
        }
        RETURN_SPACE(ResultSpace(node.GetDataWidth(), val & mask));
    }
    if (node.GetSerializationType() == "NandNode") {
        for (int i = 1; i < input_spaces.size(); i++) {
            val &= input_spaces[i].GetConstantValue();
        }
        RETURN_SPACE(ResultSpace(node.GetDataWidth(), ~val & mask));
    }
    if (node.GetSerializationType() == "XOrNode") {
        for (int i = 1; i < input_spaces.size(); i++) {
            val ^= input_spaces[i].GetConstantValue();
        }
        RETURN_SPACE(ResultSpace(node.GetDataWidth(), val & mask));
    }
    if (node.GetSerializationType() == "NorNode") {
        for (int i = 1; i < input_spaces.size(); i++) {
            val |= input_spaces[i].GetConstantValue();
        }
        RETURN_SPACE(ResultSpace(node.GetDataWidth(), ~val & mask));
    }

    CircuitError("invalid operation", node);
    ERROR_AND_RETURN;
}
void ConstExprEvaluator::visit(MultiplexerNode &node, const int output_slot) {
    const auto select = node.GetSelectInputPin().GetConnectedPin();
    // Verify connection to select pin
    VERIFY_CONNECTION(select)
    // Get input value
    const auto select_space = EvalNode(select);

    if (!select_space.IsConstant()) {
        RETURN_SPACE(ResultSpace(node.GetDataWidth()))
    }

    const int select_value = select_space.GetConstantValue();
    const auto in = node.GetInputPin(select_value).GetConnectedPin();
    VERIFY_CONNECTION(in)

    RETURN_SPACE(EvalNode(in))
}
void ConstExprEvaluator::visit(CombinerNode &node, const int output_slot) {
    std::vector<ResultSpace> input_spaces;
    input_spaces.reserve(node.GetDataWidth());

    bool all_constant = true;

    // Save each input pin value
    for (int i = 0; i < node.GetDataWidth(); i++) {
        // Input pin
        const auto in = node.GetBitInputPin(i).GetConnectedPin();
        // Verify connection to input pin
        VERIFY_CONNECTION(in)
        // Get input value
        const auto input_space = EvalNode(in);

        // Save value
        input_spaces.push_back(input_space);

        if (!input_space.IsConstant()) {
            all_constant = false;
        }
    }

    if (input_spaces.empty()) {
        CircuitError("CombinerNode has no inputs", node);
        ERROR_AND_RETURN;
    }

    if (!all_constant) {
        RETURN_SPACE(ResultSpace(node.GetDataWidth()))
    }

    uint32_t const_val = 0;
    for (int i = 0; i < node.GetDataWidth(); i++) {
        // select single bit and shift left and set
        const_val |= (1u & input_spaces[i].GetConstantValue()) << i;
    }

    const auto mask = MASK(node.GetDataWidth());
    RETURN_SPACE(ResultSpace(node.GetDataWidth(), const_val & mask))
}
void ConstExprEvaluator::visit(SplitterNode &node, const int output_slot) {
    const auto in = node.GetInputPin().GetConnectedPin();

    VERIFY_CONNECTION(in)

    const auto in_space = EvalNode(in);

    if (in_space.IsConstant()) {
        const auto in_val = in_space.GetConstantValue();

        // output_slot is the pin index (including inputs) and the first output is index 1
        const int bit_index = output_slot - 1;

        const auto bit = 0b1 & (in_val >> bit_index);

        RETURN_SPACE(ResultSpace(1, bit))
    }

    // Each output is a single bit
    RETURN_SPACE(ResultSpace(1))
}

// ========== Custom ===================================================================================================
void ConstExprEvaluator::visit(CustomModuleNode &node, const int output_slot) {
    const auto out_pin = node.GetPins().at(output_slot);

    // Assume not constant
    RETURN_SPACE(ResultSpace(out_pin.GetDataType().GetBitWidth()))
}
void ConstExprEvaluator::visit(AdderNode &node, const int output_slot) {
    const auto a = node.GetAInputPin().GetConnectedPin();
    const auto b = node.GetBInputPin().GetConnectedPin();
    const auto cin = node.GetCarryInputPin().GetConnectedPin();

    VERIFY_CONNECTION(a)
    VERIFY_CONNECTION(b)
    VERIFY_CONNECTION(cin)

    const auto a_space = EvalNode(a);
    const auto b_space = EvalNode(b);
    const auto cin_space = EvalNode(cin);

    if (a_space.IsConstant() && b_space.IsConstant() && cin_space.IsConstant()) {
        const int mask = MASK(node.GetDataWidth());
        const int a_masked = mask & a_space.GetConstantValue();
        const int b_masked = mask & b_space.GetConstantValue();
        const int cin_masked = 0b1 & cin_space.GetConstantValue();

        uint64_t wide_sum = static_cast<uint64_t>(a_masked + b_masked + cin_masked);
        uint32_t sum = static_cast<uint32_t>(wide_sum);


        if (output_slot == node.ADDER_Q_ID) {
            RETURN_SPACE(ResultSpace(node.GetDataWidth(), sum & mask))
        }
        if (output_slot == node.ADDER_COUT_ID) {
            RETURN_SPACE(ResultSpace(1, (wide_sum >> node.GetDataWidth()) & 1))
        }

        // Unknown output
        ERROR_AND_RETURN
    }


    if (output_slot == node.ADDER_Q_ID) {
        RETURN_SPACE(ResultSpace(node.GetDataWidth()))
    }
    if (output_slot == node.ADDER_COUT_ID) {
        RETURN_SPACE(ResultSpace(1))
    }

    // Unknown output
    ERROR_AND_RETURN
}
void ConstExprEvaluator::visit(ClockNode &node, const int output_slot) { RETURN_SPACE(ResultSpace(1)) }
void ConstExprEvaluator::visit(EdgeNode &node, const int output_slot) { RETURN_SPACE(ResultSpace(1)) }
void ConstExprEvaluator::visit(DebounceNode &node, const int output_slot) { RETURN_SPACE(ResultSpace(1)) }
void ConstExprEvaluator::visit(LiteralNode &node, const int output_slot) {
    RETURN_SPACE(ResultSpace(node.GetDataWidth(), node.value));
}

// ========== IO =======================================================================================================
void ConstExprEvaluator::visit(InputNode &node, const int output_slot) { RETURN_SPACE(ResultSpace(1)); }
void ConstExprEvaluator::visit(OutputNode &node, const int output_slot) {

    const auto out = node.GetValueInputPin().GetConnectedPin();
    // Verify connection to select pin
    VERIFY_CONNECTION(out)

    const auto out_space = EvalNode(out);

    if (!out_space.IsConstant()) {
        const int output_bits = node.module->GetOutputs()[node.slot].bits;
        RETURN_SPACE(ResultSpace(output_bits));
    }

    RETURN_SPACE(out_space)
}


void ConstExprEvaluator::CircuitError(const std::string &msg, const Node &node) {
    m_failed = true;
    std::cerr << "ERROR CONSTEXPR EVAL: " << msg << std::endl;
    std::cerr << "related node: " << node.guid << std::endl;
    m_error_manager->ThrowError(msg, node);
}
