//
// Created by gabed on 4/14/2026.
//

#include "Link.h"

#include <iostream>


#include "GUID.h"
#include "Module.h"
#include "Pins/Pin.h"

ImVec4 BitWidthColor(const int bits) {
    const float t = log2f(static_cast<float>(bits)) / 7.0f;
    const float hue = (1.0f - t) * 0.7f;
    return ImColor::HSV(hue, 0.7f, 0.9f);
}

Link::Link(Module *module, const std::string &output_guid, const std::string &input_guid) :
    Link(module, GUID::generate_guid(), output_guid, input_guid) {}

Link::Link(Module *module, std::string saved_id, std::string output_guid, std::string input_guid) :
    module(module), output_guid(std::move(output_guid)), input_guid(std::move(input_guid)) {
    id = GUID::to_id(std::move(saved_id));
}

void Link::Render() const {
    const auto out = module->GetPin(output_guid);
    const auto in = module->GetPin(input_guid);

    if (!out.has_value() || !in.has_value()) {
        std::cerr << "Invalid link rendering" << std::endl;
        return;
    }

    const Pin &outPin = out.value();
    const Pin &inPin = in.value();

    const auto link_color = BitWidthColor(out->GetDataType().GetBitWidth());
    const auto link_thickness = inPin.GetDataType().GetBitWidth();


    ax::NodeEditor::Link(id, outPin.GetId(), inPin.GetId(), link_color, static_cast<float>(link_thickness));
}
