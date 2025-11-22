//
// Created by Marat on 22.11.25.
//

#include "appComponent/ContactComponent.hpp"
#include "swagger/SwaggerComponent.hpp"
#include <oatpp/network/Server.hpp>
#include <iostream>

int main() {
    try {
        oatpp::base::Environment::init();

        // Register Swagger components first
        SwaggerComponent swaggerComponent;
        
        // Then register main application components
        ContactComponent component;

        OATPP_COMPONENT(std::shared_ptr<oatpp::network::ServerConnectionProvider>, serverConnectionProvider);
        OATPP_COMPONENT(std::shared_ptr<oatpp::network::ConnectionHandler>, connectionHandler);

        auto server = oatpp::network::Server::createShared(serverConnectionProvider, connectionHandler);
        std::cout << "Server running on port 8000" << '\n';
        server->run();

        oatpp::base::Environment::destroy();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        oatpp::base::Environment::destroy();
    }
}