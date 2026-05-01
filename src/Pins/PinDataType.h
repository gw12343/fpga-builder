//
// Created by Gabe on 4/20/2026.
//

#pragma once


class PinDataType {
public:
    explicit PinDataType(const int bit_width) : m_width(bit_width) {}

    [[nodiscard]] bool CanConnect(const PinDataType &other) const;

    [[nodiscard]] int GetBitWidth() const { return m_width; }

private:
    int m_width;
};
