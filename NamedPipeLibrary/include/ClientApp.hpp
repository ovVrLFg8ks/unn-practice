#ifndef CLIENTAPP_HPP
#define CLIENTAPP_HPP

#include "NamedPipeTransport.hpp"
#include "Protocol.hpp"
#include <vector>

class ClientApp {
public:
    ClientApp(const std::string& request_pipe, const std::string& response_pipe);
    void run();

private:
    NamedPipeTransport transport_;

    void sendKeepAlive();
    void sendGetStatus();
    void sendSetFrequency();
    void sendGetFrequency();
    void sendSetPower();
    void sendGetPower();
    void sendRaiseAlarm();
    void sendClearAlarm();
    void sendGetAlarms();

    void sendData(Protocol::MessageType type);
    void sendAndPrint(Protocol::MessageType type, const std::vector<uint8_t>& data);
    void printResponse(const std::vector<uint8_t>& response);
};

#endif // CLIENTAPP_HPP