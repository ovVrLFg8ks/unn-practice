#include "Client.hpp"

// returns -1 if server not answering and 0 otherwise
int SharedMemoryClient::AwaitLoop() {       
    for (int i = 0; i < awaitIter; i++) {
        if (shm.GetState() == SM_SERVER)
            usleep(sleepMs*1000);
        else
            return 0;   // server switched memory state
    }
    return -1;  // waited too long
}

void SharedMemoryClient::Ping() {
    shm.SetState(SM_SERVER);
    std::cout << shm.RecieveStreamFromServer() << std::endl;
}

void SharedMemoryClient::Exit() {
    shm.SetState(SM_SERVER);
    working = false;    // exit WorkLoop()
}

void SharedMemoryClient::Status() {
    shm.SetState(SM_SERVER);
    std::cout << shm.RecieveStreamFromServer() << std::endl;
}

void SharedMemoryClient::SetFrequency() {
    int32_t val;
    std::cin >> val;
    shm.SetNumber(val);
    shm.SetState(SM_SERVER);
    std::cout << shm.RecieveStreamFromServer() << std::endl;
}

void SharedMemoryClient::SetPower() {
    int32_t val;
    std::cin >> val;
    shm.SetNumber(val);
    shm.SetState(SM_SERVER);
    std::cout << shm.RecieveStreamFromServer() << std::endl;
}

void SharedMemoryClient::GetFrequency() {
    shm.SetState(SM_SERVER);
    if (AwaitLoop() == -1) {
        std::cout << "server not anwering" << std::endl;
        shm.SetState(SM_CLIENT);
    }
    else {
        std::cout << shm.GetNumber() << std::endl;
    }
}

void SharedMemoryClient::GetPower() {
    shm.SetState(SM_SERVER);
    if (AwaitLoop() == -1) {
        std::cout << "server not anwering" << std::endl;
        shm.SetState(SM_CLIENT);
    }
    else {
        std::cout << shm.GetNumber() << std::endl;
    }
}

void SharedMemoryClient::SetFault() {
    int32_t val;
    std::cin >> val;
    shm.SetNumber(val);
    shm.SetState(SM_SERVER);
    std::cout << shm.RecieveStreamFromServer() << std::endl;
}

void SharedMemoryClient::ClearFault() {
    int32_t val;
    std::cin >> val;
    shm.SetNumber(val);
    shm.SetState(SM_SERVER);
    std::cout << shm.RecieveStreamFromServer() << std::endl;
}

void SharedMemoryClient::ListFault() {
    shm.SetState(SM_SERVER);
    std::cout << shm.RecieveStreamFromServer() << std::endl;
}

void SharedMemoryClient::PrintCommandKeys() {
    std::cout << "available commands:\n";
    for (auto i : comms) {
        std::cout << "\t" << i.first << std::endl;
    }
}

void SharedMemoryClient::SendStr() {
    std::string s;
    std::cin >> s;
    shm.SendStreamToServer(s);
}

void SharedMemoryClient::WorkLoop() {
    working = true;
    std::string command;    // user command (ex. "ping")
    while (working) {
        std::cin >> command;
        if (shm.GetState() == SM_CLIENT) {
            if (comms.count(command) > 0) {
                shm.SetTag(comms[command].first);
                comms[command].second();
            }
            else if (command == "help")
                PrintCommandKeys();
            else
                std::cout << "wrong command" << std::endl;
        }
        usleep(sleepMs*1000);
    }
}
