//
// Created by Gabe on 4/22/2026.
//

#include "PinDataType.h"

bool PinDataType::CanConnect(const PinDataType &other) const {
    if (width != other.width)
        return false;
    return true;
}
