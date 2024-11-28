#include "NamedPipeTransport.hpp"
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <sys/stat.h>
#include <unistd.h>

NamedPipeTransport::NamedPipeTransport(const std::string& read_pipe, const std::string& write_pipe)
    : read_pipe_(read_pipe), write_pipe_(write_pipe) {
    if (access(read_pipe_.c_str(), F_OK) == -1) {
        std::cout << "Creating read pipe: " << read_pipe_ << std::endl;
        if (mkfifo(read_pipe_.c_str(), 0666) == -1) {
            std::cerr << "Error creating read pipe: " << read_pipe_ << std::endl;
            throw std::runtime_error("Failed to create read pipe");
        }
    } else {
        std::cout << "Read pipe exists: " << read_pipe_ << std::endl;
    }

    if (access(write_pipe_.c_str(), F_OK) == -1) {
        std::cout << "Creating write pipe: " << write_pipe_ << std::endl;
        if (mkfifo(write_pipe_.c_str(), 0666) == -1) {
            std::cerr << "Error creating write pipe: " << write_pipe_ << std::endl;
            throw std::runtime_error("Failed to create write pipe");
        }
    } else {
        std::cout << "Write pipe exists: " << write_pipe_ << std::endl;
    }
}

NamedPipeTransport::~NamedPipeTransport() {
    unlink(read_pipe_.c_str());
    unlink(write_pipe_.c_str());
}

void NamedPipeTransport::write(const std::vector<uint8_t>& data) {
    std::ofstream pipe(write_pipe_, std::ios::binary);
    if (!pipe.is_open()) {
        throw std::runtime_error("Failed to open write pipe");
    }
    pipe.write(reinterpret_cast<const char*>(data.data()), data.size());
    pipe.close();
}

std::vector<uint8_t> NamedPipeTransport::read() {
    std::ifstream pipe(read_pipe_, std::ios::binary);
    if (!pipe.is_open()) {
        throw std::runtime_error("Failed to open read pipe");
    }
    std::vector<uint8_t> data((std::istreambuf_iterator<char>(pipe)), std::istreambuf_iterator<char>());
    
    pipe.close();
    return data;
}