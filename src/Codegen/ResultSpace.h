//
// Created by Gabe on 5/14/2026.
//

#pragma once


class ResultSpace {
public:
    ResultSpace() : m_bits(0), m_is_constant(false), m_constant_val(0) {}
    explicit ResultSpace(const uint32_t bits) : m_bits(bits), m_is_constant(false), m_constant_val(0) {}
    ResultSpace(const uint32_t bits, const uint32_t c_val) : m_bits(bits), m_is_constant(true), m_constant_val(c_val) {}

    uint32_t GetBits() const { return m_bits; }
    bool IsConstant() const { return m_is_constant; }
    uint32_t GetConstantValue() const { return m_constant_val; }

private:
    uint32_t m_bits;
    bool m_is_constant;
    uint32_t m_constant_val;
    // TODO add spaces and limitations
};
