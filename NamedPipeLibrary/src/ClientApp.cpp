#include "ClientApp.hpp"
#include <spdlog/spdlog.h>
#include <iostream>

ClientApp::ClientApp(const std::string& request_pipe, const std::string& response_pipe)
    : transport_(request_pipe, response_pipe) {
    spdlog::info("Client initialized with pipes: Request={}, Response={}", request_pipe, response_pipe);
}

void ClientApp::run() {
    int choice;
    do {
        spdlog::info("Displaying menu to user...");
        std::cout << "1. Keep Alive\n2. Get Status\n3. Set Frequency\n4. Get Frequency\n"
                  << "5. Set Power\n6. Get Power\n7. Raise Alarm\n8. Clear Alarm\n9. Get Alarms\n0. Exit\nChoice: ";
        std::cin >> choice;

        switch (choice) {
            case 1:
                sendKeepAlive();
                break;
            case 2:
                sendGetStatus();
                break;
            case 3:
                sendSetFrequency();
                break;
            case 4:
                sendGetFrequency();
                break;
            case 5:
                sendSetPower();
                break;
            case 6:
                sendGetPower();
                break;
            case 7:
                sendRaiseAlarm();
                break;
            case 8:
                sendClearAlarm();
                break;
            case 9:
                sendGetAlarms();
                break;
            case 0:
                spdlog::info("User chose to exit.");
                std::cout << "Exiting...\n";
                break;
            default:
                spdlog::warn("Invalid menu choice: {}", choice);
        }
    } while (choice != 0);
}

void ClientApp::sendSetFrequency() {
    uint8_t value;
    std::cout << "Enter Frequency: ";
    std::cin >> value;

    auto packet = Protocol::serialize(Protocol::SET_FREQUENCY, {value});
    transport_.write(packet);
    spdlog::info("Sent Set Frequency request with value {}", value);

    auto response = transport_.read();
    auto [type, data] = Protocol::deserialize(response);
    spdlog::info("Response for Set Frequency: Type={}, Data={}", type, std::string(data.begin(), data.end()));
}

void ClientApp::sendGetFrequency() {
    auto packet = Protocol::serialize(Protocol::GET_FREQUENCY, {});
    transport_.write(packet);
    spdlog::info("Sent Get Frequency request");

    auto response = transport_.read();
    auto [type, data] = Protocol::deserialize(response);
    if (type == Protocol::GET_FREQUENCY && !data.empty()) {
        spdlog::info("Received Frequency: {}", data[0]);
        std::cout << "Frequency: " << static_cast<int>(data[0]) << "\n";
    } else {
        spdlog::warn("Failed to retrieve frequency");
    }
}

void ClientApp::sendSetPower() {
    uint8_t value;
    std::cout << "Enter Power: ";
    std::cin >> value;

    auto packet = Protocol::serialize(Protocol::SET_POWER, {value});
    transport_.write(packet);
    spdlog::info("Sent Set Power request with value {}", value);

    auto response = transport_.read();
    auto [type, data] = Protocol::deserialize(response);
    spdlog::info("Response for Set Power: Type={}, Data={}", type, std::string(data.begin(), data.end()));
}

void ClientApp::sendGetPower() {
    auto packet = Protocol::serialize(Protocol::GET_POWER, {});
    transport_.write(packet);
    spdlog::info("Sent Get Power request");

    auto response = transport_.read();
    auto [type, data] = Protocol::deserialize(response);
    if (type == Protocol::GET_POWER && !data.empty()) {
        spdlog::info("Received Power: {}", data[0]);
        std::cout << "Power: " << static_cast<int>(data[0]) << "\n";
    } else {
        spdlog::warn("Failed to retrieve power");
    }
}
void ClientApp::sendRaiseAlarm() {
    uint8_t alarm_id;
    std::cout << "Enter Alarm ID to raise: ";
    std::cin >> alarm_id;

    auto packet = Protocol::serialize(Protocol::RAISE_ALARM, {alarm_id});
    transport_.write(packet);
    spdlog::info("Sent Raise Alarm request with ID {}", alarm_id);

    auto response = transport_.read();
    auto [type, data] = Protocol::deserialize(response);

    if (type == Protocol::RAISE_ALARM && !data.empty() && data[0] == 'O') {
        spdlog::info("Alarm {} raised successfully", alarm_id);
        std::cout << "Alarm " << static_cast<int>(alarm_id) << " raised successfully.\n";
    } else {
        spdlog::warn("Failed to raise alarm {}", alarm_id);
        std::cout << "Failed to raise alarm.\n";
    }
}

void ClientApp::sendClearAlarm() {
    uint8_t alarm_id;
    std::cout << "Enter Alarm ID to clear: ";
    std::cin >> alarm_id;

    auto packet = Protocol::serialize(Protocol::CLEAR_ALARM, {alarm_id});
    transport_.write(packet);
    spdlog::info("Sent Clear Alarm request with ID {}", alarm_id);

    auto response = transport_.read();
    auto [type, data] = Protocol::deserialize(response);

    if (type == Protocol::CLEAR_ALARM && !data.empty() && data[0] == 'O') {
        spdlog::info("Alarm {} cleared successfully", alarm_id);
        std::cout << "Alarm " << static_cast<int>(alarm_id) << " cleared successfully.\n";
    } else {
        spdlog::warn("Failed to clear alarm {}", alarm_id);
        std::cout << "Failed to clear alarm.\n";
    }
}

void ClientApp::sendGetAlarms() {
    auto packet = Protocol::serialize(Protocol::GET_ALARMS, {});
    transport_.write(packet);
    spdlog::info("Sent Get Alarms request");

    auto response = transport_.read();
    auto [type, data] = Protocol::deserialize(response);

    if (type == Protocol::GET_ALARMS) {
        if (!data.empty()) {
            spdlog::info("Active alarms received: {}", fmt::join(data, ", "));
            std::cout << "Active alarms: ";
            for (uint8_t alarm_id : data) {
                std::cout << static_cast<int>(alarm_id) << " ";
            }
            std::cout << "\n";
        } else {
            spdlog::info("No active alarms received");
            std::cout << "No active alarms.\n";
        }
    } else {
        spdlog::warn("Failed to retrieve active alarms");
        std::cout << "Failed to retrieve alarms.\n";
    }
}