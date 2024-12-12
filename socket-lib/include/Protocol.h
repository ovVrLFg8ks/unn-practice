#pragma once
#include <vector>
#include <string>
#include <cstdint>
#include <cstring>
#include <stdexcept>
#include <iostream>

class ProtocolSocket {
public:
    enum Command {
        ALIVE = 1,               // Check if the server is alive
        STATUS,                  // Check the server status
        SET_FREQUENCY,          // Set the frequency on the server
        SET_POWER,              // Set the power on the server
        GET_FREQUENCY,          // Get the current frequency from the server
        GET_POWER,              // Get the current power from the server
        RAISE_EMERGENCY,        // Raise an emergency on the server
        CLEAR_EMERGENCY,        // Clear an emergency on the server
        GET_EMERGENCIES,        // Get a list of raised emergencies 
        EXIT                    // Close server and client
    };

    struct Message {
        Command command; // The command type
        std::vector<uint8_t> data;
    };

    static std::vector<uint8_t> Serialize(const Message& msg) {
        std::vector<uint8_t> buffer;
        buffer.push_back(static_cast<uint8_t>(msg.command));
        buffer.insert(buffer.end(), msg.data.begin(), msg.data.end());
        return buffer;
    }

    static Message Deserialize(const std::vector<uint8_t>& buffer) {
        if (buffer.empty()) throw std::invalid_argument("Buffer is empty.");

        Message msg;
        msg.command = static_cast<Command>(buffer[0]);
        msg.data.insert(msg.data.end(), buffer.begin() + 1, buffer.end());
        return msg;
    }
};
