#include "ClientApp.hpp"
#include <iostream>
#include <thread>
#include <chrono>

ClientApp::ClientApp(const std::string& request_pipe, const std::string& response_pipe)
    : transport_(request_pipe, response_pipe) {}

void ClientApp::run() {
    int choice;
    do {
        std::cout << "1. Keep Alive\n2. Get Status\n3. Set Frequency\n4. Get Frequency\n"
                  << "5. Set Power\n6. Get Power\n7. Raise Alarm\n8. Clear Alarm\n9. Get Alarms\n0. Exit\nChoice: ";
        std::cin >> choice;

        switch (choice) {
            case 1: sendKeepAlive(); break;
            case 2: sendGetStatus(); break;
            case 3: sendSetFrequency(); break;
            case 4: sendGetFrequency(); break;
            case 5: sendSetPower(); break;
            case 6: sendGetPower(); break;
            case 7: sendRaiseAlarm(); break;
            case 8: sendClearAlarm(); break;
            case 9: sendGetAlarms(); break;
            case 0: std::cout << "Exiting...\n"; break;
            default: std::cout << "Invalid choice\n";
        }
    } while (choice != 0);
}

void ClientApp::sendKeepAlive() { sendAndPrint(Protocol::KEEP_ALIVE, {}); }
void ClientApp::sendGetStatus() { sendAndPrint(Protocol::GET_STATUS, {}); }
void ClientApp::sendSetFrequency() { sendData(Protocol::SET_FREQUENCY); }
void ClientApp::sendGetFrequency() { sendAndPrint(Protocol::GET_FREQUENCY, {}); }
void ClientApp::sendSetPower() { sendData(Protocol::SET_POWER); }
void ClientApp::sendGetPower() { sendAndPrint(Protocol::GET_POWER, {}); }
void ClientApp::sendRaiseAlarm() { sendData(Protocol::RAISE_ALARM); }
void ClientApp::sendClearAlarm() { sendData(Protocol::CLEAR_ALARM); }
void ClientApp::sendGetAlarms() { sendAndPrint(Protocol::GET_ALARMS, {}); }

void ClientApp::sendData(Protocol::MessageType type) {
    uint8_t value;
    std::cout << "Enter value: ";
    std::cin >> value;
    auto packet = Protocol::serialize(type, {value});
    transport_.write(packet);
    auto response = transport_.read();
    printResponse(response);
}

void ClientApp::sendAndPrint(Protocol::MessageType type, const std::vector<uint8_t>& data) {
    auto packet = Protocol::serialize(type, data);
    transport_.write(packet);
    auto response = transport_.read();
    printResponse(response);
}

void ClientApp::printResponse(const std::vector<uint8_t>& response) {
    auto [type, data] = Protocol::deserialize(response);
    std::cout << "Response: Type=" << static_cast<int>(type)
              << ", Data=" << std::string(data.begin(), data.end()) << "\n";
}