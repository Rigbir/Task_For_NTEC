//
// Created by Marat on 22.11.25.
//

#pragma once

#include "dto/ContactDto.hpp"
#include "repository/ContactRepository.hpp"
#include "service/ContactService.hpp"
#include "controller/ContactController.hpp"
#include "exception/ExceptionHandler.hpp"
#include "swagger/SwaggerComponent.hpp"
#include <oatpp/web/server/handler/ErrorHandler.hpp>
#include <oatpp/core/macro/component.hpp>
#include <oatpp/web/server/HttpRouter.hpp>
#include <oatpp/parser/json/mapping/ObjectMapper.hpp>
#include <oatpp/web/server/HttpConnectionHandler.hpp>
#include <oatpp/network/tcp/server/ConnectionProvider.hpp>
#include <oatpp-swagger/Controller.hpp>
#include <oatpp-swagger/Model.hpp>

// Component for registering all application components
class ContactComponent {
public:

    // ObjectMapper for JSON serialization/deserialization
    OATPP_CREATE_COMPONENT(
        std::shared_ptr<oatpp::data::mapping::ObjectMapper>,
        objectMapper
    )([] {
        auto mapper = oatpp::parser::json::mapping::ObjectMapper::createShared();
        mapper->getDeserializer()->getConfig()->allowUnknownFields = false;
        return mapper;
    }());

    // Repository - create one instance
    OATPP_CREATE_COMPONENT(
        std::shared_ptr<ContactRepository>,
        contactRepository
    )([] {
        return std::make_shared<ContactRepository>();
    }());

    // Service - depends on Repository
    OATPP_CREATE_COMPONENT(
        std::shared_ptr<ContactService>,
        contactService
    )([] {
        OATPP_COMPONENT(std::shared_ptr<ContactRepository>, repository);
        return std::make_shared<ContactService>(repository);
    }());

    // API Error Handler - for handling exceptions in controllers
    OATPP_CREATE_COMPONENT(
        std::shared_ptr<ApiErrorHandler>,
        apiErrorHandler
    )([] {
        OATPP_COMPONENT(std::shared_ptr<oatpp::data::mapping::ObjectMapper>, objectMapper);
        return std::make_shared<ApiErrorHandler>(objectMapper);
    }());

    // Controller - depends on ObjectMapper and Service
    OATPP_CREATE_COMPONENT(
        std::shared_ptr<ContactController>,
        contactController
    )([] {
        OATPP_COMPONENT(std::shared_ptr<oatpp::data::mapping::ObjectMapper>, objectMapper);
        OATPP_COMPONENT(std::shared_ptr<ContactService>, service);
        OATPP_COMPONENT(std::shared_ptr<ApiErrorHandler>, errorHandler);
        auto controller = std::make_shared<ContactController>(objectMapper, service);
        controller->setErrorHandler(errorHandler);
        return controller;
    }());

    // Swagger Controller - depends on Controller, DocumentInfo and Resources
    // Must be created after contactController is registered
    OATPP_CREATE_COMPONENT(
        std::shared_ptr<oatpp::swagger::Controller>,
        swaggerController
    )([] {
        OATPP_COMPONENT(std::shared_ptr<ContactController>, controller);
        OATPP_COMPONENT(std::shared_ptr<oatpp::swagger::DocumentInfo>, documentInfo);
        OATPP_COMPONENT(std::shared_ptr<oatpp::swagger::Resources>, resources);
        
        // Get endpoints from controller
        oatpp::web::server::api::Endpoints docEndpoints;
        docEndpoints.append(controller->getEndpoints());
        
        return oatpp::swagger::Controller::createShared(docEndpoints, documentInfo, resources);
    }());

    // HTTP Router - for registering endpoints
    OATPP_CREATE_COMPONENT(
        std::shared_ptr<oatpp::web::server::HttpRouter>,
        httpRouter
    )([] {
        auto router = oatpp::web::server::HttpRouter::createShared();
    
        // Register Contact Controller in Router
        OATPP_COMPONENT(std::shared_ptr<ContactController>, controller);
        router->addController(controller);
        
        // Register Swagger Controller in Router
        OATPP_COMPONENT(std::shared_ptr<oatpp::swagger::Controller>, swaggerController);
        router->addController(swaggerController);
        
        return router;
    }());

    // Exception Handler
    OATPP_CREATE_COMPONENT(
        std::shared_ptr<ExceptionHandler>,
        exceptionHandler
    )([] {
        return std::make_shared<ExceptionHandler>();
    }());

    // Connection Handler - handles HTTP connections
    OATPP_CREATE_COMPONENT(
        std::shared_ptr<oatpp::network::ConnectionHandler>,
        connectionHandler
    )([] {
        OATPP_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>, httpRouter);
        OATPP_COMPONENT(std::shared_ptr<ExceptionHandler>, exceptionHandler);
        auto handler = oatpp::web::server::HttpConnectionHandler::createShared(httpRouter);
        handler->addRequestInterceptor(exceptionHandler);
        return handler;
    }());

    // Server Connection Provider - TCP server
    OATPP_CREATE_COMPONENT(
        std::shared_ptr<oatpp::network::ServerConnectionProvider>,
        serverConnectionProvider
    )([] {
        return oatpp::network::tcp::server::ConnectionProvider::createShared(
            {"0.0.0.0", 8000}, 
            oatpp::network::Address::IP_4);
    }());
};