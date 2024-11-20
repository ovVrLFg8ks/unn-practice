#include "Socket.h"

void Socket::Connect(const Address& address) {
    struct sockaddr_in addr = {0};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(address.port);
    inet_pton(AF_INET, address.host.c_str(), &addr.sin_addr);

    int res = connect(fd, (struct sockaddr*)&addr, sizeof(addr));
    if (res == -1) {
        throw std::runtime_error("Connect failed: " + std::string(strerror(errno)));
    }
}

    
void Socket::Send(const std::string& message) {
    int res = write(fd, message.c_str(), message.length());
    if (res == -1) {
        throw std::runtime_error("Write failed: " + std::string(strerror(errno)));
    }
}

    
std::string Socket::Receive() {
    char buffer[DEFAULT_BUF];
    int res = read(fd, buffer, DEFAULT_BUF);
    if (res == -1) {
        throw std::runtime_error("Read failed: " + std::string(strerror(errno)));
    } else if (res == 0) {
        return "";
    }
    return std::string(buffer, res);
}

    
void Socket::Bind(const Address& address) {
    struct sockaddr_in addr = {0};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(address.port);
    inet_pton(AF_INET, address.host.c_str(), &addr.sin_addr);

    int res = bind(fd, (struct sockaddr*)&addr, sizeof(addr));
    if (res == -1) {
        throw std::runtime_error("Bind failed: " + std::string(strerror(errno)));
    }
}

    
void Socket::Listen(int backlog) {
    int res = listen(fd, backlog);
    if (res == -1) {
        throw std::runtime_error("Listen failed: " + std::string(strerror(errno)));
    }
}

    
int Socket::Accept() {
    struct sockaddr_in addr = {0};
    socklen_t addrlen = sizeof(addr);
    int new_fd = accept(fd, (struct sockaddr*)&addr, &addrlen);
    if (new_fd == -1) {
        throw std::runtime_error("Accept failed: " + std::string(strerror(errno)));
    }
    return new_fd; 
}