//
// Created by Marat on 22.11.25.
//

#pragma once

#include "dto/ContactDto.hpp"
#include "repository/ContactRepository.hpp"
#include <memory>
#include <vector>
#include <stdexcept>

// Service layer for delegating repository work before API level (Controller)
// Here data validity is checked before passing them to the repository
// So that the Endpoint interface in Controller is not cluttered with data validation logic
// And connection with the low-level CRUD operations layer (Repository)
class ContactService {
public:
    explicit ContactService(const std::shared_ptr<ContactRepository>& repository)
        : repository_(repository) {}

    oatpp::Object<ContactDto> createContact(const oatpp::Object<ContactDto>& contact) {
        validateContact(contact, false);

        if (contact->id && *contact->id < 0) {
            throw std::runtime_error("Invalid ID: ID must be positive or zero");
        }
        
        auto result = repository_->create(contact);
        if (!result) {
            throw std::runtime_error("Failed to create contact: ID already exists");
        }
        return result;
    }

    oatpp::Object<ContactDto> getContactById(oatpp::Int64 id) {
        if (id <= 0) {
            throw std::runtime_error("Invalid ID");
        }
        auto result = repository_->getById(id);
        if (!result) {
            throw std::runtime_error("Contact not found");
        }
        return result;
    }

    std::vector<oatpp::Object<ContactDto>> getAllContacts() {
        return repository_->getAll();
    }

    oatpp::Object<ContactDto> updateContact(const oatpp::Object<ContactDto>& contact) {
        validateContact(contact, true);
        auto result = repository_->update(contact);
        if (!result) {
            throw std::runtime_error("Contact not found");
        }
        return result;
    }

    bool deleteContact(oatpp::Int64 id) {
        if (id <= 0) {
            throw std::runtime_error("Invalid ID");
        }
        return repository_->remove(id);
    }
private:
    std::shared_ptr<ContactRepository> repository_;

    void validateContact(const oatpp::Object<ContactDto>& contact, bool requiredId) {
        if (requiredId && (!contact->id || *contact->id <= 0)) {
            throw std::runtime_error("Valid ID is required");
        }

        if (!contact->name || contact->name->empty()) {
            throw std::runtime_error("Name is required");
        }

        if (!contact->phone || contact->phone->empty()) {
            throw std::runtime_error("Phone is required");
        }

        if (!contact->address || contact->address->empty()) {
            throw std::runtime_error("Address is required");
        }
    }
};