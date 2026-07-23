cmake_minimum_required(VERSION 3.14)
project(lustre_iscsi_mgmt CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

file(GLOB SOURCES "src/*.cpp")

add_executable(lustre_mgmt ${SOURCES})

# OpenSSL required for SHA-256 direct file integrity checks
find_package(OpenSSL REQUIRED)
target_link_libraries(lustre_mgmt PRIVATE OpenSSL::Crypto)
