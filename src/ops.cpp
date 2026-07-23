#include "io_ops.hpp"
#include <fcntl.h>
#include <unistd.h>
#include <openssl/sha.h>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <cstdlib>

bool IOOps::write_direct_io(const std::string& filepath, const std::string& data) {
    std::cout << "[+] Performing Direct I/O write (O_DIRECT) to " << filepath << "...\n";
    
    // O_DIRECT requires block-aligned memory allocations
    size_t alignment = 4096;
    size_t size = ((data.size() + alignment - 1) / alignment) * alignment;
    
    void* buffer = nullptr;
    if (posix_memalign(&buffer, alignment, size) != 0) {
        std::cerr << "[-] Memory alignment failed.\n";
        return false;
    }
    
    std::memset(buffer, 0, size);
    std::memcpy(buffer, data.c_str(), data.size());

    int fd = open(filepath.c_str(), O_WRONLY | O_CREAT | O_DIRECT | O_TRUNC, 0644);
    if (fd < 0) {
        std::perror("[-] open (O_DIRECT) failed");
        free(buffer);
        return false;
    }

    ssize_t written = write(fd, buffer, size);
    close(fd);
    free(buffer);

    return written > 0;
}

bool IOOps::evict_cache(const std::string& filepath) {
    std::cout << "[+] Forcefully evicting page cache via posix_fadvise...\n";
    int fd = open(filepath.c_str(), O_RDONLY);
    if (fd < 0) return false;

    int res = posix_fadvise(fd, 0, 0, POSIX_FADV_DONTNEED);
    close(fd);
    return res == 0;
}

std::string IOOps::calculate_sha256(const std::string& filepath) {
    std::ifstream file(filepath, std::ios::binary);
    if (!file) return "";

    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    char buffer[4096];

    while (file.read(buffer, sizeof(buffer)) || file.gcount()) {
        SHA256_Update(&sha256, buffer, file.gcount());
    }

    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_Final(hash, &sha256);

    std::stringstream ss;
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
    }

    return ss.str();
}
