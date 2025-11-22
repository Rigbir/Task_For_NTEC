//
// Created by Marat on 22.11.25.
//

#pragma once

#include "repository/ContactRepository.hpp"
#include "dto/ContactDto.hpp"
#include <oatpp-test/UnitTest.hpp>
#include <oatpp/core/base/Environment.hpp>

namespace test {

class ContactRepositoryTest : public oatpp::test::UnitTest {
public:
    ContactRepositoryTest() : UnitTest("TEST[ContactRepositoryTest]") {}

    void onRun() override {
        OATPP_LOGI(TAG, "  [1/10] Testing create contact...");
        // Test create contact
        {
            auto repository = std::make_shared<ContactRepository>();
            auto contact = ContactDto::createShared();
            // Don't set id - will be auto-generated
            contact->name = "Test User";
            contact->phone = "+79991234567";
            contact->address = "Test Address";

            auto created = repository->create(contact);
            OATPP_ASSERT(created != nullptr);
            OATPP_ASSERT(created->id != nullptr);
            OATPP_ASSERT(*created->id > 0);
            OATPP_ASSERT(created->name == "Test User");
            OATPP_ASSERT(created->phone == "+79991234567");
            OATPP_ASSERT(created->address == "Test Address");
        }

        OATPP_LOGI(TAG, "  [2/10] Testing getById...");
        // Test getById
        {
            auto repository = std::make_shared<ContactRepository>();
            auto contact = ContactDto::createShared();
            // Don't set id - will be auto-generated
            contact->name = "Test User 2";
            contact->phone = "+79997654321";
            contact->address = "Test Address 2";

            auto created = repository->create(contact);
            OATPP_ASSERT(created != nullptr);
            OATPP_ASSERT(created->id != nullptr);
            auto id = *created->id;

            auto retrieved = repository->getById(id);
            OATPP_ASSERT(retrieved != nullptr);
            OATPP_ASSERT(retrieved->id != nullptr);
            OATPP_ASSERT(*retrieved->id == id);
            OATPP_ASSERT(retrieved->name == "Test User 2");
            OATPP_ASSERT(retrieved->phone == "+79997654321");
            OATPP_ASSERT(retrieved->address == "Test Address 2");
        }

        OATPP_LOGI(TAG, "  [3/10] Testing getById with non-existent ID...");
        // Test getById with non-existent ID
        {
            auto repository = std::make_shared<ContactRepository>();
            auto retrieved = repository->getById(99999);
            OATPP_ASSERT(retrieved == nullptr);
        }

        OATPP_LOGI(TAG, "  [4/10] Testing getAll...");
        // Test getAll
        {
            auto repository = std::make_shared<ContactRepository>();
            auto allContacts = repository->getAll();
            OATPP_ASSERT(allContacts.size() >= 3); // At least seeded data
        }

        OATPP_LOGI(TAG, "  [5/10] Testing update...");
        // Test update
        {
            auto repository = std::make_shared<ContactRepository>();
            auto contact = ContactDto::createShared();
            // Don't set id - will be auto-generated
            contact->name = "Original Name";
            contact->phone = "+79991111111";
            contact->address = "Original Address";

            auto created = repository->create(contact);
            OATPP_ASSERT(created != nullptr);
            OATPP_ASSERT(created->id != nullptr);
            auto id = *created->id;

            auto updated = ContactDto::createShared();
            updated->id = id;
            updated->name = "Updated Name";
            updated->phone = "+79992222222";
            updated->address = "Updated Address";

            auto result = repository->update(updated);
            OATPP_ASSERT(result != nullptr);
            OATPP_ASSERT(result->id != nullptr);
            OATPP_ASSERT(*result->id == id);
            OATPP_ASSERT(result->name == "Updated Name");
            OATPP_ASSERT(result->phone == "+79992222222");
            OATPP_ASSERT(result->address == "Updated Address");
        }

        OATPP_LOGI(TAG, "  [6/10] Testing update with non-existent ID...");
        // Test update with non-existent ID
        {
            auto repository = std::make_shared<ContactRepository>();
            auto contact = ContactDto::createShared();
            contact->id = 99999;
            contact->name = "Test";
            contact->phone = "+79991111111";
            contact->address = "Test Address";

            auto result = repository->update(contact);
            OATPP_ASSERT(result == nullptr);
        }

        OATPP_LOGI(TAG, "  [7/10] Testing remove...");
        // Test remove
        {
            auto repository = std::make_shared<ContactRepository>();
            auto contact = ContactDto::createShared();
            // Don't set id - will be auto-generated
            contact->name = "To Delete";
            contact->phone = "+79993333333";
            contact->address = "Delete Address";

            auto created = repository->create(contact);
            OATPP_ASSERT(created != nullptr);
            OATPP_ASSERT(created->id != nullptr);
            auto id = *created->id;

            bool deleted = repository->remove(id);
            OATPP_ASSERT(deleted);

            auto retrieved = repository->getById(id);
            OATPP_ASSERT(retrieved == nullptr);
        }

        OATPP_LOGI(TAG, "  [8/10] Testing remove with non-existent ID...");
        // Test remove with non-existent ID
        {
            auto repository = std::make_shared<ContactRepository>();
            bool deleted = repository->remove(99999);
            OATPP_ASSERT(!deleted);
        }

        OATPP_LOGI(TAG, "  [9/10] Testing create with explicit ID...");
        // Test create with explicit ID
        {
            auto repository = std::make_shared<ContactRepository>();
            auto contact = ContactDto::createShared();
            contact->id = 100;
            contact->name = "Explicit ID";
            contact->phone = "+79994444444";
            contact->address = "Explicit Address";

            auto created = repository->create(contact);
            OATPP_ASSERT(created != nullptr);
            OATPP_ASSERT(created->id != nullptr);
            OATPP_ASSERT(*created->id == 100);
        }

        OATPP_LOGI(TAG, "  [10/10] Testing create with duplicate ID...");
        // Test create with duplicate ID
        {
            auto repository = std::make_shared<ContactRepository>();
            auto contact1 = ContactDto::createShared();
            contact1->id = 200;
            contact1->name = "First";
            contact1->phone = "+79995555555";
            contact1->address = "First Address";

            auto created1 = repository->create(contact1);
            OATPP_ASSERT(created1 != nullptr);

            auto contact2 = ContactDto::createShared();
            contact2->id = 200;
            contact2->name = "Second";
            contact2->phone = "+79996666666";
            contact2->address = "Second Address";

            auto created2 = repository->create(contact2);
            OATPP_ASSERT(created2 == nullptr); // Should fail due to duplicate ID
        }
    }
};

}

