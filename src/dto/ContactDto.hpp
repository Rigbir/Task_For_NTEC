//
// Created by Marat on 22.11.25.
//

#pragma once

#include <oatpp/core/Types.hpp>
#include <oatpp/core/macro/codegen.hpp>

#include OATPP_CODEGEN_BEGIN(DTO)

// Data structure for contact
class ContactDto: public oatpp::DTO {
    DTO_INIT(ContactDto, DTO);

    DTO_FIELD(Int64,  id, "id");
    DTO_FIELD(String, name, "name");
    DTO_FIELD(String, phone, "phone");
    DTO_FIELD(String, address, "address");
};

#include OATPP_CODEGEN_END(DTO)