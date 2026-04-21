//
// Created by gabed on 4/14/2026.
//

#include "Link.h"

#include <iostream>

#include "GUID.h"
#include "Module.h"
#include "Pin.h"

Link::Link(Module *module, const std::string &output_guid, const std::string &input_guid) :
    Link(module, GUID::generate_guid(), output_guid, input_guid) {}

Link::Link(Module *module, std::string saved_id, const std::string &output_guid, const std::string &input_guid) :
    module(module), output_guid(output_guid), input_guid(input_guid) {
    id = GUID::to_id(saved_id);
}

void Link::Render() const {
    const auto out = module->GetPin(output_guid);
    const auto in = module->GetPin(input_guid);

    if (!out.has_value() || !in.has_value()) {
        std::cerr << "Invalid link rendering" << std::endl;
        return;
    }

    const Pin outPin = out.value();
    const Pin inPin = in.value();

    ed::Link(id, outPin.GetId(), inPin.GetId(), ImVec4(0, 255, 0, 255), 2.0f);
}
