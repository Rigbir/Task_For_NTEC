//
// Created by Marat on 22.11.25.
//

#pragma once

#include "dto/ContactDto.hpp"
#include <atomic>
#include <mutex>
#include <vector>
#include <unordered_map>
#include <oatpp/core/Types.hpp>

// Repository for working with contacts in memory (basic CRUD operations)
// First layer for working with data before Service level
// Could use a database, but for a test task this is sufficient
class ContactRepository {
public:
    ContactRepository(): nextId_(1) {
        seedTestData();
    }

    // oatpp::Object<ContactDto> <=> std::shared_ptr<ContactDto>
    oatpp::Object<ContactDto> create(const oatpp::Object<ContactDto>& contact) {
        std::lock_guard<std::mutex> lock(mutex_);

        auto newContact = ContactDto::createShared();
        newContact->name = contact->name;
        newContact->phone = contact->phone;
        newContact->address = contact->address;

        if (!contact->id || *contact->id == 0) {
            newContact->id = ++nextId_;
        } else {
            auto idValue = *contact->id;
            if (storage_.contains(idValue)) {
                return nullptr;
            }
            newContact->id = idValue;
            nextId_ = std::max(nextId_.load(), idValue + 1);
        }

        storage_[newContact->id] = newContact;
        return newContact;
    }

    oatpp::Object<ContactDto> getById(oatpp::Int64 id) {
        std::lock_guard<std::mutex> lock(mutex_);
        if (storage_.contains(id)) {
            auto contact = ContactDto::createShared();
            contact->id = id;
            contact->name = storage_[id]->name;
            contact->phone = storage_[id]->phone;
            contact->address = storage_[id]->address;
            return contact;
        }
        return nullptr;
    }

    std::vector<oatpp::Object<ContactDto>> getAll() {
        std::lock_guard<std::mutex> lock(mutex_);
        std::vector<oatpp::Object<ContactDto>> result;
        result.reserve(storage_.size());

        for (const auto& pair : storage_) {
            auto contact = ContactDto::createShared();
            contact->id = pair.second->id;
            contact->name = pair.second->name;
            contact->phone = pair.second->phone;
            contact->address = pair.second->address;
            result.push_back(std::move(contact));
        }

        return result;
    }

    oatpp::Object<ContactDto> update(const oatpp::Object<ContactDto>& contact) {
        std::lock_guard<std::mutex> lock(mutex_);

        auto it = storage_.find(contact->id);
        if (it == storage_.end()) {
            return nullptr;
        }

        it->second->name = contact->name;
        it->second->phone = contact->phone;
        it->second->address = contact->address;

        auto updated = ContactDto::createShared();
        updated->id = it->second->id;
        updated->name = it->second->name;
        updated->phone = it->second->phone;
        updated->address = it->second->address;
        return updated;
    }

    bool remove(oatpp::Int64 id) {
        std::lock_guard<std::mutex> lock(mutex_);
        return storage_.erase(id) > 0;
    }

private:
    std::unordered_map<oatpp::Int64, oatpp::Object<ContactDto>> storage_;
    std::mutex mutex_;
    std::atomic<int64_t> nextId_;

    void seedTestData() {
        auto contact1 = ContactDto::createShared();
        contact1->id = 1;
        contact1->name = oatpp::String("Ivan Ivanov");
        contact1->phone = oatpp::String("+79991234567");
        contact1->address = oatpp::String("Moscow, Lenin St., 1");
        storage_[1] = contact1;

        auto contact2 = ContactDto::createShared();
        contact2->id = 2;
        contact2->name = oatpp::String("Maria Petrova");
        contact2->phone = oatpp::String("+79997654321");
        contact2->address = oatpp::String("Saint Petersburg, Nevsky Ave., 10");
        storage_[2] = contact2;

        auto contact3 = ContactDto::createShared();
        contact3->id = 3;
        contact3->name = oatpp::String("Alexey Sidorov");
        contact3->phone = oatpp::String("+79995555555");
        contact3->address = oatpp::String("Kazan, Bauman St., 5");
        storage_[3] = contact3;

        nextId_ = 4;
    }
};