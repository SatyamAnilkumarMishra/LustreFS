LustreFS on Network Block Storage
A low-level C++17 distributed storage management engine for automated SAN provisioning, high-performance filesystem deployment, direct cache management, stress benchmarking, and self-healing fault recovery.

📋 Table of Contents
Overview

Supported Operating Systems

Key Features
1.System Architecture
2.Project Directory Structure
3.Prerequisites & Dependencies
4.Building & Installation
5.Execution & Verification
6.Pipeline Execution Steps
7.Benchmarking & Logs
8.Troubleshooting & FAQ

🔎 Overview
In enterprise datacenters and High-Performance Computing (HPC) environments, distributed storage stacks (like Lustre) are disaggregated from compute nodes and served over high-speed Storage Area Networks (SANs). Managing these tiers manually requires coordinating dozens of kernel daemons, iSCSI targets, initiator sessions, and mount points.

LustreFS on Network Block Storage encapsulates this entire lifecycle into a modular, Object-Oriented C++ engine. It automates raw backstore creation, iSCSI target exports (targetcli), initiator discovery/binding (iscsiadm), Lustre MGT/MDT formatting (with Ext4 fallback), non-volatile page cache eviction (posix_fadvise), automated stress workloads (fio), and live session fault-recovery testing.

🐧 Supported Operating Systems
This framework is optimized and verified for enterprise Linux environments:

Ubuntu 22.04 LTS (Jammy Jellyfish)

Red Hat Enterprise Linux (RHEL 8 / RHEL 9) (and binary-compatible derivatives like AlmaLinux & Rocky Linux)

✨ Key Features
Automated SAN Target Export: Dynamically provisions raw backing files and configures LIO iSCSI target IQNs and LUN ACLs via C++ wrappers.

Dynamic Block Device Binding: Executes target discovery over loopback/network interfaces (127.0.0.1) and maps block targets directly into the kernel device tree (/dev/sdb).

Resilient Filesystem Deployment: Formats devices using native Lustre MGT/MDT rules (mkfs.lustre) with an automated Ext4 fallback for standard Linux distributions.

Hardware Cache Eviction & Hash Verification: Bypasses filesystem memory caching via posix_fadvise system calls to verify direct non-volatile block persistence and checksum integrity.

Automated Workload Benchmarking: Programmatically generates and executes multi-job fio workloads to record IOPS, throughput, and latency metrics.

Self-Healing Fault Recovery: Features a simulated network fault-injector that drops active target sessions and verifies immediate client re-discovery without manual operator intervention.

🏗️ System Architecture
  ┌────────────────────────────────────────────────────────────────────────┐
  │                           main.cpp (Pipeline Runner)                   │
  └────────────────────────────────────────────────────────────────────────┘
            │               │               │               │
            ▼               ▼               ▼               ▼
     ┌─────────────┐ ┌─────────────┐ ┌─────────────┐ ┌─────────────┐
     │ dependency  │ │   target    │ │  initiator  │ │   manager   │
     │ (.hpp/.cpp) │ │ (.hpp/.cpp) │ │ (.hpp/.cpp) │ │ (.hpp/.cpp) │
     └─────────────┘ └─────────────┘ └─────────────┘ └─────────────┘
      Audit Tooling   iSCSI Export    SAN Discovery   Format/Mount
            │               │               │               │
            ▼               ▼               ▼               ▼
     ┌─────────────┐ ┌─────────────┐ ┌─────────────┐ ┌─────────────┐
     │  benchmark  │ │  injector   │ │     ops     │ │    utils    │
     │ (.hpp/.cpp) │ │ (.hpp/.cpp) │ │ (.hpp/.cpp) │ │ (.hpp/.cpp) │
     └─────────────┘ └─────────────┘ └─────────────┘ └─────────────┘
      FIO Testing     Fault Drop      System Exec     Logging/Paths

📁 Project Directory Structure
LustreFS-main/
├── CMakeLists.txt            # CMake build orchestration rules
├── install.sh                # Automated build and compilation script
├── include/                  # C++ Header Files
│   ├── benchmark.hpp         # Workload generator & FIO benchmark suite
│   ├── dependency.hpp        # Host binary & system audit helper
│   ├── initiator.hpp         # iSCSI Initiator discovery & session binding
│   ├── injector.hpp          # Simulated fault-injection & session drops
│   ├── manager.hpp           # Filesystem format, mount, and cache eviction
│   ├── ops.hpp              # Process execution & subprocess abstractions
│   ├── target.hpp            # iSCSI Target & LIO backstore export engine
│   └── utils.hpp             # Helper utilities, paths, and formatting
└── src/                      # C++ Source Files
    ├── main.cpp              # 8-step pipeline execution controller
    ├── benchmark.cpp         # FIO test suite implementation
    ├── dependency.cpp        # Dependency check implementation
    ├── initiator.cpp        # Open-iSCSI client implementation
    ├── injector.cpp         # Network fault simulation engine
    ├── manager.cpp           # Mounts, formatting, and cache eviction logic
    ├── ops.cpp               # Low-level process runner logic
    ├── target.cpp            # Targetcli LIO driver wrapper
    └── utils.cpp             # Path management & SHA-256 verification


🛠️ Prerequisites & Dependencies
For Ubuntu 22.04 LTS

Bash
sudo apt update
sudo apt install -y build-essential cmake targetcli-fb open-iscsi fio


For RHEL 8 / 9

Bash

sudo dnf update -y
sudo dnf groupinstall -y "Development Tools"
sudo dnf install -y cmake targetcli iscsi-initiator-utils fio

# Start and enable the iSCSI daemon on RHEL
sudo systemctl enable --now iscsid



🔨 Building & Installation
Clone the Repository:

Bash
git clone https://github.com/SatyamAnilkumarMishra/LustreFS.git
cd LustreFS

Run Automated Build Script:
The included install.sh script automates directory creation, CMake configuration, and g++ compilation[cite: 3]:

Bash
chmod +x install.sh
sudo ./install.sh

Verify Binary Output:
After compilation completes, the primary executable will be generated at ./build/lustre_mgmt.

🚀 Execution & Verification
Because SAN provisioning and kernel block device binding require elevated privileges, run the runner using sudo:

Bash
sudo ./build/lustre_mgmt

Manual Inspection Commands Post-Run
Once execution completes, you can inspect the generated targets and session states:

Bash
# 1. View created raw backing images
ls -lh /tmp/iscsi_store*.img

# 2. Inspect active LIO target backstores
sudo targetcli ls /backstores/fileio

# 3. Check connected iSCSI sessions
sudo iscsiadm -m session

# 4. Inspect block device tree and active mounts
lsblk
df -h /mnt/lustre_mdt

🔄 Pipeline Execution Steps
When invoked, lustre_mgmt sequentially runs through the 8 storage lifecycle steps:

Dependency Audit (dependency.cpp): Audits host environment for targetcli, iscsiadm, fio, and mkfs.lustre[cite: 3].

Target Provisioning (target.cpp): Allocates /tmp/iscsi_store.img and exports it as IQN iqn.2026-07.com.storage:lustre-block[cite: 3].

Initiator Binding (initiator.cpp): Discovers loopback targets (127.0.0.1) and maps target storage as block device /dev/sdb[cite: 3].

Filesystem Provisioning (manager.cpp): Formats /dev/sdb as Lustre MGT/MDT (or Ext4 fallback) and mounts to /mnt/lustre_mdt[cite: 3].

Direct Cache Eviction (manager.cpp): Writes direct blocks, triggers posix_fadvise page-cache eviction, and verifies SHA-256 data hash[cite: 3].

Stress Benchmarking (benchmark.cpp): Generates multi-threaded read/write workloads using fio[cite: 3].

Fault Injection & Recovery (injector.cpp): Simulates session drops and verifies auto-healing client re-discovery[cite: 3].

Pipeline Verification: Aggregates logs and validates execution completeness.

📊 Benchmarking & Logs
All benchmark metrics generated during Step 6 are saved to the project log directory:

Bash
cat ./benchmark_logs/fio_results.log

