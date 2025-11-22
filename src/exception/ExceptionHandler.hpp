//
// Created by Marat on 22.11.25.
//

#pragma once

#include "dto/ErrorDto.hpp"
#include <oatpp/web/server/interceptor/RequestInterceptor.hpp>
#include <oatpp/web/server/handler/ErrorHandler.hpp>
#include <oatpp/web/protocol/http/outgoing/ResponseFactory.hpp>
#include <oatpp/web/protocol/http/Http.hpp>
#include <oatpp/parser/json/mapping/ObjectMapper.hpp>
#include <stdexcept>
#include <string>

// Centralized exception handler for API.
// Converts exceptions and errors to standardized ErrorDto responses.
class ApiErrorHandler : public oatpp::web::server::handler::ErrorHandler {
private:
    std::shared_ptr<oatpp::data::mapping::ObjectMapper> objectMapper_;

    oatpp::web::protocol::http::Status determineStatus(const std::string& message) {
        if (message == "Contact not found") {
            return oatpp::web::protocol::http::Status::CODE_404;
        } else if (message.find("Invalid ID") != std::string::npos ||
                   message.find("required") != std::string::npos ||
                   message.find("Failed to create") != std::string::npos ||
                   message.find("ID already exists") != std::string::npos ||
                   message.find("must be positive") != std::string::npos) {
            return oatpp::web::protocol::http::Status::CODE_400;
        }
        return oatpp::web::protocol::http::Status::CODE_500;
    }

    std::string getStatusMessage(const oatpp::web::protocol::http::Status& status) {
        if (status.code == 404) {
            return "Not Found";
        } else if (status.code == 400) {
            return "Bad Request";
        }
        return "Internal Server Error";
    }

public:
    explicit ApiErrorHandler(const std::shared_ptr<oatpp::data::mapping::ObjectMapper>& objectMapper)
        : objectMapper_(objectMapper) {}

    std::shared_ptr<oatpp::web::protocol::http::outgoing::Response>
    handleError(const oatpp::web::protocol::http::Status& status,
                const oatpp::String& message,
                const Headers& headers) override {
        auto error = ErrorDto::createShared();
        
        std::string messageStr = message ? message->c_str() : "";
        auto determinedStatus = determineStatus(messageStr);
        
        // Use determined status from message if provided status is generic (500), otherwise use provided
        auto finalStatus = (status.code == 500) ? determinedStatus : status;
        
        error->status = finalStatus.code;
        error->message = getStatusMessage(finalStatus);
        error->details = messageStr.empty() ? "Unknown error" : messageStr;

        return oatpp::web::protocol::http::outgoing::ResponseFactory::createResponse(
            finalStatus, error, objectMapper_);
    }
};

// Request interceptor for logging/monitoring (optional)
class ExceptionHandler : public oatpp::web::server::interceptor::RequestInterceptor {
public:
    std::shared_ptr<OutgoingResponse> intercept(
        const std::shared_ptr<IncomingRequest>& request) override {
        // Just pass through - exceptions are handled by ApiErrorHandler
        return nullptr;
    }
};