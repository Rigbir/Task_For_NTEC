//
// Created by Marat on 22.11.25.
//

#include "ContactRepositoryTest.hpp"
#include "ContactServiceTest.hpp"
#include <oatpp/core/base/Environment.hpp>
#include <iostream>

int main() {
    oatpp::base::Environment::init();

    std::cout << "\n==========================================\n";
    std::cout << "Running Unit Tests\n";
    std::cout << "==========================================\n\n";

    OATPP_RUN_TEST(test::ContactRepositoryTest);
    OATPP_RUN_TEST(test::ContactServiceTest);

    std::cout << "\n==========================================\n";
    std::cout << "All Tests Completed\n";
    std::cout << "==========================================\n\n";

    oatpp::base::Environment::destroy();

    return 0;
}

