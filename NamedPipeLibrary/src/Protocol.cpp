#include "Protocol.hpp"
#include <stdexcept>

std::vector<uint8_t> Protocol::serialize(MessageType type, const std::vector<uint8_t>& data) {
    std::vector<uint8_t> packet;
    packet.push_back(static_cast<uint8_t>(type));
    packet.insert(packet.end(), data.begin(), data.end());
    return packet;
}

std::pair<Protocol::MessageType, std::vector<uint8_t>> Protocol::deserialize(const std::vector<uint8_t>& packet) {
    if (packet.empty()) {
        throw std::runtime_error("Empty packet");
    }
    MessageType type = static_cast<MessageType>(packet[0]);
    std::vector<uint8_t> data(packet.begin() + 1, packet.end());
    return {type, data};
}