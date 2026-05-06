//
// Created by Gabe on 4/22/2026.
//

#include "PinDataType.h"

bool PinDataType::CanConnect(const PinDataType &other) const {
    if (m_width != other.m_width)
        return false;
    return true;
}
