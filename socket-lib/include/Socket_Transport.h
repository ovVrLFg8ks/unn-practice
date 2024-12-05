#include "Client.h"
#include <iostream>
#include <string>

class Transport{
private:
    bool working = true; 
public:

    Transport() : address(DEFAULT_PORT, DEFAULT_HOST) {}
    void Stop_Socket() { working = false; }
    void Run(){
        working = true;
        while (working) {
            Client client(address);                 // Client object responsible for network communication
            client.ShowCommands();          // Show all available commands
            std::cin >> command;

            if (command == 0) {
                dlog::info("Exit...");
                // Send EXIT command to the server
                Protocol::Message exit_msg;
                exit_msg.command = Protocol::EXIT;  
                client.SendRequest(exit_msg.command, exit_msg.data);
                break;  
            }
            // Prepare the message based on user input
            Protocol::Message request = client.HandleCommand(command);

            try {
                client.SendRequest(request.command, request.data);      // Send the prepared request to the server
                std::string response = client.ReceiveResponse();        // Receive the response from the server

                client.ProcessServerResponse(command, response);

            } catch (const std::exception& e) {
                dlog::error("Error sending/receiving data: " + std::string(e.what()));
            }
        }
    }
private:
    Address address;    // Default address of the server
    int command = -1;

};