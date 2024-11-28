#include "ServerApp.hpp"
#include <iostream>
#include <thread>
#include <chrono>
#include <map>

ServerApp::ServerApp(const std::string& request_pipe, const std::string& response_pipe)
    : transport_(request_pipe, response_pipe), frequency_(0), power_(0) {}

void ServerApp::run() {
    std::cout << "Server started, waiting for requests..." << std::endl;

    while (true) {
        try {
            auto request = transport_.read();
            auto [type, data] = Protocol::deserialize(request);
            handleRequest(type, data);
        } catch (const std::exception& e) {
            std::cerr << "Error processing request: " << e.what() << std::endl;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void ServerApp::handleRequest(const Protocol::MessageType& type, const std::vector<uint8_t>& data) {
    switch (type) {
        case Protocol::KEEP_ALIVE:
            transport_.write(Protocol::serialize(Protocol::KEEP_ALIVE));
            std::cout << "Handled Keep Alive\n";
            break;

        case Protocol::GET_STATUS:
            transport_.write(Protocol::serialize(Protocol::GET_STATUS, {'O', 'K'}));
            std::cout << "Handled Get Status\n";
            break;

        case Protocol::SET_FREQUENCY:
            if (!data.empty()) {
                frequency_ = data[0];
                transport_.write(Protocol::serialize(Protocol::SET_FREQUENCY));
                std::cout << "Set Frequency to " << static_cast<int>(frequency_) << "\n";
            }
            break;

        case Protocol::SET_POWER:
            if (!data.empty()) {
                power_ = data[0];
                transport_.write(Protocol::serialize(Protocol::SET_POWER));
                std::cout << "Set Power to " << static_cast<int>(power_) << "\n";
            }
            break;

        case Protocol::GET_FREQUENCY:
            transport_.write(Protocol::serialize(Protocol::GET_FREQUENCY, {frequency_}));
            std::cout << "Reported Frequency: " << static_cast<int>(frequency_) << "\n";
            break;

        case Protocol::GET_POWER:
            transport_.write(Protocol::serialize(Protocol::GET_POWER, {power_}));
            std::cout << "Reported Power: " << static_cast<int>(power_) << "\n";
            break;

        case Protocol::RAISE_ALARM:
            if (!data.empty()) {
                uint8_t alarm_id = data[0];
                alarms_.insert(alarm_id);
                transport_.write(Protocol::serialize(Protocol::RAISE_ALARM));
                std::cout << "Raised Alarm " << static_cast<int>(alarm_id) << "\n";
            }
            break;

        case Protocol::CLEAR_ALARM:
            if (!data.empty()) {
                uint8_t alarm_id = data[0];
                alarms_.erase(alarm_id);
                transport_.write(Protocol::serialize(Protocol::CLEAR_ALARM));
                std::cout << "Cleared Alarm " << static_cast<int>(alarm_id) << "\n";
            }
            break;

        case Protocol::GET_ALARMS: {
            std::vector<uint8_t> alarms_list(alarms_.begin(), alarms_.end());
            transport_.write(Protocol::serialize(Protocol::GET_ALARMS, alarms_list));
            std::cout << "Sent list of active alarms\n";
            break;
        }

        default:
            std::cerr << "Unknown message type\n";
    }
}