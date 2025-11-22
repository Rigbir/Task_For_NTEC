//
// Created by Marat on 22.11.25.
//

#pragma once

#include "dto/ContactDto.hpp"
#include "dto/ErrorDto.hpp"
#include "service/ContactService.hpp"
#include <memory>
#include <stdexcept>
#include <oatpp/web/server/api/ApiController.hpp>
#include <oatpp/parser/json/mapping/ObjectMapper.hpp>
#include <oatpp-swagger/Model.hpp>
#include <oatpp-swagger/Resources.hpp>

#include OATPP_CODEGEN_BEGIN(ApiController)

// Controller for working with contacts
// Here mapping of requests and responses between API and Service layers occurs
// Here data validation also occurs before passing them to the Service layer
// And mapping data from Service layer to API responses
class ContactController: public oatpp::web::server::api::ApiController {
public:
    explicit ContactController(const std::shared_ptr<oatpp::data::mapping::ObjectMapper>& objectMapper,
                               const std::shared_ptr<ContactService>& service)
    : ApiController(objectMapper)
    , service_(service) {}

    ENDPOINT_INFO(createContact) {
        info->summary = "Create a new contact";
        info->description = "Create a new contact in the phone directory";
        info->addConsumes<oatpp::Object<ContactDto>>("application/json");
        info->addResponse<oatpp::Object<ContactDto>>(Status::CODE_201, "application/json", "Contact created successfully");
        info->addResponse<oatpp::Object<ErrorDto>>(Status::CODE_400, "application/json", "Bad Request");
    }
    ENDPOINT("POST", "contacts", createContact,
         BODY_DTO(oatpp::Object<ContactDto>, contactDto)) {
        auto contact = service_->createContact(contactDto);
        return createDtoResponse(Status::CODE_201, contact);
    }

    ENDPOINT_INFO(getContactById) {
        info->summary = "Get contact by ID";
        info->description = "Retrieve a contact from the phone directory by its ID";
        info->pathParams["id"].description = "Contact identifier";
        info->addResponse<oatpp::Object<ContactDto>>(Status::CODE_200, "application/json", "Contact found");
        info->addResponse<oatpp::Object<ErrorDto>>(Status::CODE_404, "application/json", "Contact not found");
        info->addResponse<oatpp::Object<ErrorDto>>(Status::CODE_400, "application/json", "Bad Request");
    }
    ENDPOINT("GET", "contacts/{id}", getContactById,
             PATH(oatpp::Int64, id)) {
        auto contact = service_->getContactById(id);
        return createDtoResponse(Status::CODE_200, contact);
    }

    ENDPOINT_INFO(getAllContacts) {
        info->summary = "Get all contacts";
        info->description = "Retrieve all contacts from the phone directory";
        info->addResponse<oatpp::List<oatpp::Object<ContactDto>>>(Status::CODE_200, "application/json", "List of contacts");
        info->addResponse<oatpp::Object<ErrorDto>>(Status::CODE_500, "application/json", "Internal Server Error");
    }
    ENDPOINT("GET", "contacts", getAllContacts) {
        auto contacts = service_->getAllContacts();
        auto response = oatpp::List<oatpp::Object<ContactDto>>::createShared();
        for (const auto& contact : contacts) {
            response->push_back(std::move(contact));
        }
        return createDtoResponse(Status::CODE_200, response);
    }

    ENDPOINT_INFO(updateContact) {
        info->summary = "Update contact by ID";
        info->description = "Update an existing contact in the phone directory";
        info->pathParams["id"].description = "Contact identifier";
        info->addConsumes<oatpp::Object<ContactDto>>("application/json");
        info->addResponse<oatpp::Object<ContactDto>>(Status::CODE_200, "application/json", "Contact updated successfully");
        info->addResponse<oatpp::Object<ErrorDto>>(Status::CODE_404, "application/json", "Contact not found");
        info->addResponse<oatpp::Object<ErrorDto>>(Status::CODE_400, "application/json", "Bad Request");
    }
    ENDPOINT("PUT", "contacts/{id}", updateContact,
             PATH(oatpp::Int64, id),
             BODY_DTO(oatpp::Object<ContactDto>, contactDto)) {
        contactDto->id = id;
        auto contact = service_->updateContact(contactDto);
        return createDtoResponse(Status::CODE_200, contact);
    }

    ENDPOINT_INFO(deleteContact) {
        info->summary = "Delete contact by ID";
        info->description = "Delete a contact from the phone directory by its ID";
        info->pathParams["id"].description = "Contact identifier";
        info->addResponse<String>(Status::CODE_204, "text/plain", "Contact deleted successfully");
        info->addResponse<oatpp::Object<ErrorDto>>(Status::CODE_404, "application/json", "Contact not found");
        info->addResponse<oatpp::Object<ErrorDto>>(Status::CODE_400, "application/json", "Bad Request");
    }
    ENDPOINT("DELETE", "contacts/{id}", deleteContact,
             PATH(oatpp::Int64, id)) {
        bool deleted = service_->deleteContact(id);
        if (!deleted) {
            throw std::runtime_error("Contact not found");
        }
        return createResponse(Status::CODE_204, "");
    }

private:
    std::shared_ptr<ContactService> service_;
};

#include OATPP_CODEGEN_END(ApiController)