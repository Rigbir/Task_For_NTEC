//
// Created by Marat on 22.11.25.
//

#pragma once

#include <oatpp/core/macro/component.hpp>
#include <oatpp-swagger/Model.hpp>
#include <oatpp-swagger/Resources.hpp>

// Swagger components configuration
// Note: swaggerController is created in ContactComponent because it depends on ContactController
class SwaggerComponent {
public:

    // Swagger Document Info
    OATPP_CREATE_COMPONENT(
        std::shared_ptr<oatpp::swagger::DocumentInfo>,
        swaggerDocumentInfo
    )([] {
        oatpp::swagger::DocumentInfo::Builder builder;
        builder
            .setTitle("Phone Directory API")
            .setDescription("REST API for managing phone directory contacts")
            .setVersion("1.0")
            .setContactName("API Support")
            .setContactUrl("https://oatpp.io/")
            .setLicenseName("Apache License, Version 2.0")
            .setLicenseUrl("http://www.apache.org/licenses/LICENSE-2.0")
            .addServer("http://localhost:8000", "server on localhost");
        return builder.build();
    }());

    // Swagger Resources
    OATPP_CREATE_COMPONENT(
        std::shared_ptr<oatpp::swagger::Resources>,
        swaggerResources
    )([] {
        // Path to swagger-ui resources - defined by CMake
        #ifdef OATPP_SWAGGER_RES_PATH
        return oatpp::swagger::Resources::streamResources(OATPP_SWAGGER_RES_PATH);
        #else
        // Fallback path (relative to build directory)
        return oatpp::swagger::Resources::streamResources("_deps/oatpp-swagger-src/res");
        #endif
    }());
};

