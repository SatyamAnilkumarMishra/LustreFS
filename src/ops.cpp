#define OPENSSL_SUPPRESS_DEPRECATED
#include "ops.hpp"
#include <cstring>      
#include <iostream>
#include <fstream>
#include <fcntl.h>
#include <unistd.h>
#include <iomanip>
#include <sstream>
#include <openssl/sha.h>

bool IOOps::write_direct_io(const std::string& filepath, const std::string& data) {
    int fd = open(filepath.c_str(), O_WRONLY | O_CREAT | O_TRUNC | O_DIRECT, 0644);
    if (fd < 0) {
        std::cerr << "[-] Error opening file for Direct I/O: " << filepath << std::endl;
        return false;
    }

    size_t alignment = 512;
    size_t size = (data.size() + alignment - 1) & ~(alignment - 1);
    void* buffer = nullptr;

    if (posix_memalign(&buffer, alignment, size) != 0) {
        std::cerr << "[-] Memory alignment allocation failed." << std::endl;
        close(fd);
        return false;
    }

    std::memset(buffer, 0, size);
    std::memcpy(buffer, data.c_str(), data.size());

    ssize_t bytes_written = write(fd, buffer, size);
    free(buffer);
    close(fd);

    return bytes_written > 0;
}

bool IOOps::evict_cache(const std::string& filepath) {
    int fd = open(filepath.c_str(), O_RDONLY);
    if (fd < 0) return false;

    int res = posix_fadvise(fd, 0, 0, POSIX_FADV_DONTNEED);
    close(fd);
    return res == 0;
}

std::string IOOps::calculate_sha256(const std::string& filepath) {
    std::ifstream file(filepath, std::ios::binary);
    if (!file.is_open()) return "";

    SHA256_CTX sha256;
    SHA256_Init(&sha256);

    char buffer[4096];
    while (file.read(buffer, sizeof(buffer))) {
        SHA256_Update(&sha256, buffer, file.gcount());
    }
    SHA256_Update(&sha256, buffer, file.gcount());

    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_Final(hash, &sha256);

    std::stringstream ss;
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
    }

    return ss.str();
}
