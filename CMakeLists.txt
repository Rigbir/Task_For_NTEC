cmake_minimum_required(VERSION 3.14)

project(Task_For_NTEC)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set policy version minimum for dependencies
set(CMAKE_POLICY_VERSION_MINIMUM 3.5)

# Always use FetchContent for oatpp to ensure correct architecture and get oatpp-test
include(FetchContent)
FetchContent_Declare(
    oatpp
    GIT_REPOSITORY https://github.com/oatpp/oatpp.git
    GIT_TAG 1.3.0
)
FetchContent_MakeAvailable(oatpp)
message(STATUS "Using oatpp from FetchContent")
set(OATPP_TEST_LIB oatpp-test)

# Always fetch oatpp-swagger (it's not in Homebrew)
include(FetchContent)
FetchContent_Declare(
    oatpp-swagger
    GIT_REPOSITORY https://github.com/oatpp/oatpp-swagger.git
    GIT_TAG 1.3.0
)
FetchContent_MakeAvailable(oatpp-swagger)

# Add executable
add_executable(${PROJECT_NAME}
    src/main.cpp
)

# Define OATPP_SWAGGER_RES_PATH for Swagger resources
set(OATPP_SWAGGER_RES_PATH "${CMAKE_BINARY_DIR}/_deps/oatpp-swagger-src/res")
target_compile_definitions(${PROJECT_NAME} PRIVATE OATPP_SWAGGER_RES_PATH="${OATPP_SWAGGER_RES_PATH}")

# Include directories
target_include_directories(${PROJECT_NAME}
    PRIVATE src
)

# Link libraries
# When using FetchContent with add_subdirectory, use 'oatpp' not 'oatpp::oatpp'
target_link_libraries(${PROJECT_NAME}
    PRIVATE oatpp
    PRIVATE oatpp-swagger
)

# Add test executable
enable_testing()

add_executable(${PROJECT_NAME}_tests
    tests/AllTestsMain.cpp
)

target_include_directories(${PROJECT_NAME}_tests
    PRIVATE src
    PRIVATE tests
)

target_link_libraries(${PROJECT_NAME}_tests
    PRIVATE ${OATPP_TEST_LIB}
    PRIVATE oatpp
)

add_test(NAME ${PROJECT_NAME}_tests COMMAND ${PROJECT_NAME}_tests)
