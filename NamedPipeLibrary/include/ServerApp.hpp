#ifndef SERVERAPP_HPP
#define SERVERAPP_HPP

#include "NamedPipeTransport.hpp"
#include "Protocol.hpp"
#include <set>
#include <vector>
#include <cstdint>

class ServerApp {
public:
    ServerApp(const std::string& request_pipe, const std::string& response_pipe);
    void run();

private:
    NamedPipeTransport transport_;
    
    uint8_t frequency_;
    uint8_t power_;
    std::set<uint8_t> alarms_;

    void handleRequest(const Protocol::MessageType& type, const std::vector<uint8_t>& data);
};

#endif // SERVERAPP_HPP