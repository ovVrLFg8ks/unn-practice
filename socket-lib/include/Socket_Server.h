#pragma once
#include "Socket.h"
#include "Protocol.h"
#include <iostream>
#include <algorithm>
#include <vector>

class Server {
public:
    Server(const Address& address, int backlog = 5) : socket(AF_INET, SOCK_STREAM, 0) {
        socket.Bind(address);  
        socket.Listen(backlog);  
    }

    // This method accepts a client connection, processes its request, and sends a response
    void Accept_client() {
        int new_fd = socket.Accept();  
        Socket new_socket(new_fd);     

        // Receive the client's message (the request) as a string and convert it to a vector of bytes
        std::string client_msg = new_socket.Receive();  
        std::vector<uint8_t> buffer(client_msg.begin(), client_msg.end());

        Protocol::Message request = Protocol::Deserialize(buffer);  

        Protocol::Message response;
        response.command = request.command;

        switch (request.command) {
            case Protocol::ALIVE:
                response.data = {'O', 'K'};
                break;
            case Protocol::STATUS:
                response.data = {'G', 'O', 'O', 'D'};
                break;
            case Protocol::SET_FREQUENCY: {
                if (request.data.size() == sizeof(float)) {
                     // Convert the data (byte array) into a float and store it in the server's current frequency variable
                    std::memcpy(&current_frequency, request.data.data(), sizeof(float));
                }
                response.data = {'O', 'K'};
                break;
            }
            case Protocol::SET_POWER: {
                if (request.data.size() == sizeof(float)) {
                    std::memcpy(&current_power, request.data.data(), sizeof(float));
                }
                response.data = {'O', 'K'};
                break;
            }
            case Protocol::GET_FREQUENCY: {
                response.data.resize(sizeof(float));  
                // Copy the current frequency value into the response data
                std::memcpy(response.data.data(), &current_frequency, sizeof(float));  
                break;
            }
            case Protocol::GET_POWER: {
                response.data.resize(sizeof(float));  
                std::memcpy(response.data.data(), &current_power, sizeof(float));  
                break;
            }
            case Protocol::RAISE_EMERGENCY: {
                if (request.data.size() == sizeof(int)) {
                     // Extract the emergency number (int) from the data and add it to the emergencies list
                    int emergency = *reinterpret_cast<int*>(request.data.data());
                    emergencies.push_back(emergency);
                }
                response.data = {'O', 'K'};
                break;
            }
            case Protocol::CLEAR_EMERGENCY: {
                if (request.data.size() == sizeof(int)) {
                    // Extract the emergency number (int) from the data and remove it from the emergencies list
                    int emergency = *reinterpret_cast<int*>(request.data.data());
                    emergencies.erase(std::remove(emergencies.begin(), emergencies.end(), emergency), emergencies.end());
                }
                response.data = {'O', 'K'};
                break;
            }
            case Protocol::GET_EMERGENCIES: {
                response.data.resize(emergencies.size() * sizeof(int));
                // Copy the list of emergencies into the response data
                std::memcpy(response.data.data(), emergencies.data(), emergencies.size() * sizeof(int));
                break;
            }
            case Protocol::EXIT: {
                std::cout << "Received EXIT command. Shutting down server." << std::endl;
                exit(0);  
            }
            default:
                response.data = {'E', 'R', 'R', 'O', 'R'};
                break;
        }
        // Serialize the response message into a byte array
        auto serialized_response = Protocol::Serialize(response);  
        // Send the serialized response back to the client
        new_socket.Send(std::string(serialized_response.begin(), serialized_response.end()));  

        std::cout << "Processed command: " << static_cast<int>(request.command) << std::endl;

        close(new_fd);
    }

    void Run() {
    // Enter an infinite loop to continuously accept client connections
        std::cout << "Server started, waiting for connections..." << std::endl;
        while (true) {
            try {
                // Accept a client connection and process its request
                Accept_client();
            }catch(const std::exception& e) {
                std::cerr << "Error during communication: " << e.what() << std::endl;
                break;
            }
        }
    }

private:
    Socket socket;  // The server's listening socket
    float current_frequency = 50.0f;  // The current frequency value (default 50.0)
    float current_power = 100.0f;     // The current power value (default 100.0)
    std::vector<int> emergencies;     // A list to track raised emergencies   
};
