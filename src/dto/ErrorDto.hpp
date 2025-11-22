//
// Created by Marat on 22.11.25.
//

#pragma once

#include <oatpp/core/Types.hpp>
#include <oatpp/core/macro/codegen.hpp>

#include OATPP_CODEGEN_BEGIN(DTO)

// Data structure for API errors
class ErrorDto : public oatpp::DTO {
  DTO_INIT(ErrorDto, DTO);

  DTO_FIELD(Int32,  status, "status");
  DTO_FIELD(String, message, "message");
  DTO_FIELD(String, details, "details");
};

#include OATPP_CODEGEN_END(DTO)