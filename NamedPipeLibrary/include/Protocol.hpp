#ifndef PROTOCOL_HPP
#define PROTOCOL_HPP

#include <vector>
#include <string>
#include <cstdint>

class Protocol {
public:
    enum MessageType : uint8_t {
        KEEP_ALIVE = 0x01,
        GET_STATUS = 0x02,
        SET_FREQUENCY = 0x03,
        SET_POWER = 0x04,
        GET_FREQUENCY = 0x05,
        GET_POWER = 0x06,
        RAISE_ALARM = 0x07,
        CLEAR_ALARM = 0x08,
        GET_ALARMS = 0x09
    };

    static std::vector<uint8_t> serialize(MessageType type, const std::vector<uint8_t>& data = {});
    static std::pair<MessageType, std::vector<uint8_t>> deserialize(const std::vector<uint8_t>& packet);
};

#endif // PROTOCOL_HPP