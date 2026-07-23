#!/usr/bin/env bash

# Exit immediately if any command fails
set -e

GREEN='\033[0;32m'
YELLOW='\033[1;33m'
RED='\033[0;31m'
NC='\033[0m'

echo -e "${GREEN}=======================================================${NC}"
echo -e "${GREEN}  LUSTRE & iSCSI MANAGEMENT TOOL - INSTALLATION SCRIPT ${NC}"
echo -e "${GREEN}=======================================================${NC}\n"

# 1. Require Root / Sudo Privileges
if [ "$EUID" -ne 0 ]; then
  echo -e "${RED}[!] Error: This script must be run with sudo privileges.${NC}"
  echo -e "    Please run: ${YELLOW}sudo ./install.sh${NC}"
  exit 1
fi

# 2. System Package Audit & Installation (Ubuntu 22.04)
echo -e "${YELLOW}[1/5] Updating package list & installing system dependencies...${NC}"
apt-get update -y
apt-get install -y \
    build-essential \
    cmake \
    libssl-dev \
    targetcli-fb \
    open-iscsi \
    fio \
    e2fsprogs \
    util-linux \
    pkg-config

# 3. System Services Configuration
echo -e "\n${YELLOW}[2/5] Enabling iSCSI & Target daemon services...${NC}"
systemctl enable --now iscsid || true
systemctl enable --now targetcli || true

# 4. Prepare Mount & Log Directory Hierarchy
echo -e "\n${YELLOW}[3/5] Setting up system directories...${NC}"
mkdir -p /mnt/lustre_mdt
mkdir -p /mnt/lustre_client
mkdir -p benchmark_logs

# 5. Check Project Files & Generate CMakeLists.txt
echo -e "\n${YELLOW}[4/5] Verifying source files and configuring CMake...${NC}"

cat << 'EOF' > CMakeLists.txt
cmake_minimum_required(VERSION 3.16)
project(lustre_mgmt CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find required OpenSSL / Crypto library for SHA-256
find_package(OpenSSL REQUIRED)

# Search root directory, include/, and src/ for header files
include_directories(. include src "${CMAKE_CURRENT_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}/src")

# Collect all source files dynamically
file(GLOB SOURCES 
    "src/*.cpp" 
    "*.cpp"
)

add_executable(lustre_mgmt ${SOURCES})

target_link_libraries(lustre_mgmt PRIVATE OpenSSL::Crypto pthread)
EOF

echo "[+] Updated CMakeLists.txt to compile project modules."

# 6. Build Project Binary
echo -e "\n${YELLOW}[5/5] Compiling C++ binary...${NC}"
rm -rf build
mkdir -p build
cd build

cmake ..
make -j$(nproc)

echo -e "\n${GREEN}=======================================================${NC}"
echo -e "${GREEN}  [✓] BUILD COMPLETE!                                  ${NC}"
echo -e "${GREEN}=======================================================${NC}\n"
echo -e "To run the automated pipeline directly, execute:"
echo -e "  ${YELLOW}sudo ./build/lustre_mgmt${NC}\n"
