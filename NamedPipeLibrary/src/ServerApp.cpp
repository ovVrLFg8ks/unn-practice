#include "ServerApp.hpp"
#include <spdlog/spdlog.h>
#include <thread>

ServerApp::ServerApp(const std::string& request_pipe, const std::string& response_pipe)
    : transport_(request_pipe, response_pipe), frequency_(0), power_(0) {}

void ServerApp::run() {
    spdlog::info("Server started, waiting for requests...");
    while (true) {
        try {
            auto request = transport_.read();
            spdlog::info("Request received");

            auto [type, data] = Protocol::deserialize(request);
            handleRequest(type, data);
        } catch (const std::exception& e) {
            spdlog::error("Error reading from pipe: {}", e.what());
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void ServerApp::handleRequest(const Protocol::MessageType& type, const std::vector<uint8_t>& data) {
    switch (type) {
        case Protocol::KEEP_ALIVE:
            transport_.write(Protocol::serialize(Protocol::KEEP_ALIVE, {}));
            spdlog::info("Handled Keep Alive");
            break;

        case Protocol::GET_STATUS:
            transport_.write(Protocol::serialize(Protocol::GET_STATUS, {'O', 'K'}));
            spdlog::info("Handled Get Status");
            break;

        case Protocol::GET_FREQUENCY:
            transport_.write(Protocol::serialize(Protocol::GET_FREQUENCY, {frequency_}));
            spdlog::info("Reported Frequency: {}", frequency_);
            break;

        case Protocol::SET_FREQUENCY:
            if (!data.empty()) {
                frequency_ = data[0];
                transport_.write(Protocol::serialize(Protocol::SET_FREQUENCY, {'O', 'K'}));
                spdlog::info("Set Frequency to {}", frequency_);
            } else {
                transport_.write(Protocol::serialize(Protocol::SET_FREQUENCY, {'N', 'O', 'K'}));
                spdlog::warn("Failed to set frequency: No data provided");
            }
            break;

        case Protocol::GET_POWER:
            transport_.write(Protocol::serialize(Protocol::GET_POWER, {power_}));
            spdlog::info("Reported Power: {}", power_);
            break;

        case Protocol::SET_POWER:
            if (!data.empty()) {
                power_ = data[0];
                transport_.write(Protocol::serialize(Protocol::SET_POWER, {'O', 'K'}));
                spdlog::info("Set Power to {}", power_);
            } else {
                transport_.write(Protocol::serialize(Protocol::SET_POWER, {'N', 'O', 'K'}));
                spdlog::warn("Failed to set power: No data provided");
            }
            break;

        case Protocol::RAISE_ALARM:
            if (!data.empty()) {
                alarms_.insert(data[0]);
                transport_.write(Protocol::serialize(Protocol::RAISE_ALARM, {'O', 'K'}));
                spdlog::info("Raised Alarm: {}", data[0]);
            } else {
                transport_.write(Protocol::serialize(Protocol::RAISE_ALARM, {'N', 'O', 'K'}));
                spdlog::warn("Failed to raise alarm: No data provided");
            }
            break;

        case Protocol::CLEAR_ALARM:
            if (!data.empty()) {
                alarms_.erase(data[0]);
                transport_.write(Protocol::serialize(Protocol::CLEAR_ALARM, {'O', 'K'}));
                spdlog::info("Cleared Alarm: {}", data[0]);
            } else {
                transport_.write(Protocol::serialize(Protocol::CLEAR_ALARM, {'N', 'O', 'K'}));
                spdlog::warn("Failed to clear alarm: No data provided");
            }
            break;

        case Protocol::GET_ALARMS: {
            std::vector<uint8_t> alarms_list(alarms_.begin(), alarms_.end());
            transport_.write(Protocol::serialize(Protocol::GET_ALARMS, alarms_list));
            spdlog::info("Sent list of active alarms");
            break;
        }

        default:
            spdlog::warn("Unhandled request type: {}", static_cast<int>(type));
            break;
    }
}