#pragma once
#include "Socket.h"
#include "Protocol.h"
#include <vector>
#include <string>

class Client {
public:
    Client(const Address& address) : socket(AF_INET, SOCK_STREAM, 0) {
        socket.Connect(address);
    }

    
    void SendRequest(Protocol::Command cmd, const std::vector<uint8_t>& data = {}) {
        // Create a message containing the command and the data to be sent
        Protocol::Message msg{cmd, data};               
        auto serialized = Protocol::Serialize(msg);    

        // Send the serialized message as a string over the socket 
        socket.Send(std::string(serialized.begin(), serialized.end()));  
    }

    
    std::string ReceiveResponse() {
        
        std::string response = socket.Receive();
        // Convert the received string into a vector of bytes for deserialization
        std::vector<uint8_t> buffer(response.begin(), response.end());
        
        try {
            Protocol::Message msg = Protocol::Deserialize(buffer);
            // Convert the message data into a string 
            std::string responseStr(msg.data.begin(), msg.data.end());
            return responseStr;
        } catch (const std::exception& e) {
            dlog::error("Error during deserialization: " + std::string(e.what()));
            return "";
        }
    }

    // Show available commands to the user
    void ShowCommands() const {
        dlog::info ("Choose a command: \n")  ;
        dlog::info ("1: I'm alive \n") ;
        dlog::info ("2: Tell me your status \n" );
        dlog::info ("3: Set frequency (value) \n") ;
        dlog::info ("4: Set power (value) \n") ;
        dlog::info ("5: Tell me frequency \n" );
        dlog::info ("6: Tell me power \n") ;
        dlog::info ("7: Raise emergency (number) \n" );
        dlog::info ("8: Clear emergency (number) \n" );
        dlog::info ("9: Tell me the list of raised emergencies \n" );
        dlog::info ("0: Exit \n" );
        dlog::info ("Enter command: ");
    }

    // Handle user input and prepare the corresponding protocol message for the server.
    Protocol::Message HandleCommand(int command) {
        Protocol::Message request;
        switch (command) {
            case 1: 
                request.command = Protocol::ALIVE;
                break;
            case 2: 
                request.command = Protocol::STATUS;
                break;
            case 3: { 
                dlog::info ("Enter frequency: ");
                std::cin >> frequency;

                request.command = Protocol::SET_FREQUENCY;
                request.data.resize(sizeof(float));  
                std::memcpy(request.data.data(), &frequency, sizeof(float));  // Copy the frequency value into the request data
                break;
            }
            case 4: { 
                dlog::info ("Enter power: ");
                std::cin >> power;

                request.command = Protocol::SET_POWER;
                request.data.resize(sizeof(float));
                std::memcpy(request.data.data(), &power, sizeof(float)); // Copy the power value into the request data
                break;
            }
            case 5: 
                request.command = Protocol::GET_FREQUENCY;
                break;
            case 6: 
                request.command = Protocol::GET_POWER;
                break;
            case 7: { 
                int emergency;
                dlog::info ("Enter emergency number: ");
                std::cin >> emergency;
                request.command = Protocol::RAISE_EMERGENCY;
                request.data.resize(sizeof(int));
                std::memcpy(request.data.data(), &emergency, sizeof(int)); // Copy the emergency number into the request data
                break;
            }
            case 8: { 
                int emergency;
                dlog::info ("Enter emergency number to clear: ");
                std::cin >> emergency;
                request.command = Protocol::CLEAR_EMERGENCY;
                request.data.resize(sizeof(int));
                std::memcpy(request.data.data(), &emergency, sizeof(int)); // Copy the emergency number to clear into the request data
                break;
            }
            case 9: 
                request.command = Protocol::GET_EMERGENCIES;
                break;
            default:
                dlog::info ("Invalid command! \n" );
                break ;
        }
        return request;
    }

    // Process the server response based on the command
    void ProcessServerResponse(int command, const std::string& response) const {
        if (command == 5) {  
            float received_frequency;
            std::memcpy(&received_frequency, response.data(), sizeof(float));
            dlog::info("Server response (Frequency): " + std::to_string(received_frequency));
        }
        else if (command == 6) {  
            float received_power;
            std::memcpy(&received_power, response.data(), sizeof(float));
            dlog::info("Server response (Power): " + std::to_string(received_power));
        } 
        else if (command == 9) {  
            if (!response.empty()) {
                std::string emergencies_str;
                for (size_t i = 0; i < response.size() / sizeof(int); ++i) {
                    int emergency;
                    std::memcpy(&emergency, &response[i * sizeof(int)], sizeof(int));
                    emergencies_str += std::to_string(emergency) + " ";
                }
                dlog::info("Server response (Emergencies): " + emergencies_str);
            } else {
                dlog::info ("No emergencies raised. \n" );
            }
        } 
        else {
            dlog::info("Server response: " + response);
        }
    }

    
private:
    Socket socket; 
    float frequency;
    float power;  
};
