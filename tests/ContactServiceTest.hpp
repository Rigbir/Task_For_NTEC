//
// Created by Marat on 22.11.25.
//

#pragma once

#include "service/ContactService.hpp"
#include "repository/ContactRepository.hpp"
#include "dto/ContactDto.hpp"
#include <oatpp-test/UnitTest.hpp>
#include <oatpp/core/base/Environment.hpp>
#include <stdexcept>

namespace test {

class ContactServiceTest : public oatpp::test::UnitTest {
public:
    ContactServiceTest() : UnitTest("TEST[ContactServiceTest]") {}

    void onRun() override {
        auto repository = std::make_shared<ContactRepository>();
        auto service = std::make_shared<ContactService>(repository);

        OATPP_LOGI(TAG, "  [1/14] Testing create contact...");
        // Test create contact
        {
            auto contact = ContactDto::createShared();
            contact->name = "Service Test User";
            contact->phone = "+79997777777";
            contact->address = "Service Test Address";

            auto created = service->createContact(contact);
            OATPP_ASSERT(created != nullptr);
            OATPP_ASSERT(created->id != nullptr);
            OATPP_ASSERT(*created->id > 0);
            OATPP_ASSERT(created->name == "Service Test User");
        }

        OATPP_LOGI(TAG, "  [2/14] Testing create contact with missing name...");
        // Test create contact with missing name
        {
            auto contact = ContactDto::createShared();
            contact->phone = "+79998888888";
            contact->address = "Test Address";

            try {
                service->createContact(contact);
                OATPP_ASSERT(false); // Should throw
            } catch (const std::runtime_error& e) {
                OATPP_ASSERT(std::string(e.what()).find("required") != std::string::npos);
            }
        }

        OATPP_LOGI(TAG, "  [3/14] Testing create contact with missing phone...");
        // Test create contact with missing phone
        {
            auto contact = ContactDto::createShared();
            contact->name = "Test User";
            contact->address = "Test Address";

            try {
                service->createContact(contact);
                OATPP_ASSERT(false); // Should throw
            } catch (const std::runtime_error& e) {
                OATPP_ASSERT(std::string(e.what()).find("required") != std::string::npos);
            }
        }

        OATPP_LOGI(TAG, "  [4/14] Testing create contact with missing address...");
        // Test create contact with missing address
        {
            auto contact = ContactDto::createShared();
            contact->name = "Test User";
            contact->phone = "+79999999999";

            try {
                service->createContact(contact);
                OATPP_ASSERT(false); // Should throw
            } catch (const std::runtime_error& e) {
                OATPP_ASSERT(std::string(e.what()).find("required") != std::string::npos);
            }
        }

        OATPP_LOGI(TAG, "  [5/14] Testing getContactById...");
        // Test getContactById
        {
            auto contact = ContactDto::createShared();
            contact->name = "Get By ID Test";
            contact->phone = "+79991000000";
            contact->address = "Get By ID Address";

            auto created = service->createContact(contact);
            OATPP_ASSERT(created->id != nullptr);
            auto id = *created->id;

            auto retrieved = service->getContactById(id);
            OATPP_ASSERT(retrieved != nullptr);
            OATPP_ASSERT(retrieved->id != nullptr);
            OATPP_ASSERT(*retrieved->id == id);
            OATPP_ASSERT(retrieved->name == "Get By ID Test");
        }

        OATPP_LOGI(TAG, "  [6/14] Testing getContactById with invalid ID...");
        // Test getContactById with invalid ID
        {
            try {
                service->getContactById(-1);
                OATPP_ASSERT(false); // Should throw
            } catch (const std::runtime_error& e) {
                OATPP_ASSERT(std::string(e.what()).find("Invalid ID") != std::string::npos);
            }
        }

        OATPP_LOGI(TAG, "  [7/14] Testing getContactById with non-existent ID...");
        // Test getContactById with non-existent ID
        {
            try {
                service->getContactById(99999);
                OATPP_ASSERT(false); // Should throw
            } catch (const std::runtime_error& e) {
                OATPP_ASSERT(std::string(e.what()) == "Contact not found");
            }
        }

        OATPP_LOGI(TAG, "  [8/14] Testing getAllContacts...");
        // Test getAllContacts
        {
            auto contacts = service->getAllContacts();
            OATPP_ASSERT(contacts.size() > 0);
        }

        OATPP_LOGI(TAG, "  [9/14] Testing updateContact...");
        // Test updateContact
        {
            auto contact = ContactDto::createShared();
            contact->name = "Original";
            contact->phone = "+79991111111";
            contact->address = "Original Address";

            auto created = service->createContact(contact);
            OATPP_ASSERT(created->id != nullptr);
            auto id = *created->id;

            auto updated = ContactDto::createShared();
            updated->id = id;
            updated->name = "Updated";
            updated->phone = "+79992222222";
            updated->address = "Updated Address";

            auto result = service->updateContact(updated);
            OATPP_ASSERT(result != nullptr);
            OATPP_ASSERT(result->id != nullptr);
            OATPP_ASSERT(*result->id == id);
            OATPP_ASSERT(result->name == "Updated");
            OATPP_ASSERT(result->phone == "+79992222222");
        }

        OATPP_LOGI(TAG, "  [10/14] Testing updateContact with invalid ID...");
        // Test updateContact with invalid ID
        {
            auto contact = ContactDto::createShared();
            contact->id = -1;
            contact->name = "Test";
            contact->phone = "+79993333333";
            contact->address = "Test Address";

            try {
                service->updateContact(contact);
                OATPP_ASSERT(false); // Should throw
            } catch (const std::runtime_error& e) {
                OATPP_ASSERT(std::string(e.what()).find("required") != std::string::npos ||
                            std::string(e.what()).find("Invalid ID") != std::string::npos);
            }
        }

        OATPP_LOGI(TAG, "  [11/14] Testing updateContact with non-existent ID...");
        // Test updateContact with non-existent ID
        {
            auto contact = ContactDto::createShared();
            contact->id = 99999;
            contact->name = "Test";
            contact->phone = "+79994444444";
            contact->address = "Test Address";

            try {
                service->updateContact(contact);
                OATPP_ASSERT(false); // Should throw
            } catch (const std::runtime_error& e) {
                OATPP_ASSERT(std::string(e.what()) == "Contact not found");
            }
        }

        OATPP_LOGI(TAG, "  [12/14] Testing deleteContact...");
        // Test deleteContact
        {
            auto contact = ContactDto::createShared();
            contact->name = "To Delete";
            contact->phone = "+79995555555";
            contact->address = "Delete Address";

            auto created = service->createContact(contact);
            OATPP_ASSERT(created->id != nullptr);
            auto id = *created->id;

            bool deleted = service->deleteContact(id);
            OATPP_ASSERT(deleted);

            try {
                service->getContactById(id);
                OATPP_ASSERT(false); // Should throw
            } catch (const std::runtime_error& e) {
                OATPP_ASSERT(std::string(e.what()) == "Contact not found");
            }
        }

        OATPP_LOGI(TAG, "  [13/14] Testing deleteContact with invalid ID...");
        // Test deleteContact with invalid ID
        {
            try {
                service->deleteContact(-1);
                OATPP_ASSERT(false); // Should throw
            } catch (const std::runtime_error& e) {
                OATPP_ASSERT(std::string(e.what()).find("Invalid ID") != std::string::npos);
            }
        }

        OATPP_LOGI(TAG, "  [14/14] Testing deleteContact with non-existent ID...");
        // Test deleteContact with non-existent ID
        {
            bool deleted = service->deleteContact(99999);
            OATPP_ASSERT(!deleted);
        }
    }
};

}