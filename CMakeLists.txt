cmake_minimum_required(VERSION 3.16)
project(lustre_mgmt CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenSSL REQUIRED)

include_directories(. include src "${CMAKE_CURRENT_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}/src")

file(GLOB SOURCES 
    "src/*.cpp" 
    "*.cpp"
)

add_executable(lustre_mgmt ${SOURCES})

target_link_libraries(lustre_mgmt PRIVATE OpenSSL::Crypto pthread)
