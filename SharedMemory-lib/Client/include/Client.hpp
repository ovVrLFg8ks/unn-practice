#pragma once

#include "SharedMemoryCore.hpp"

#include <iostream>
#include <map>
#include <vector>
#include <functional>

class SharedMemoryClient {
protected:
    SharedMemory shm;

    bool working = true;    // WorkLoop() state
    int sleepMs = 333;      // WorkLoop() sleep in loop

    int awaitIter = 10;     // loops max number of iteratoins (in case of server not answering)

    int AwaitLoop();

public:
    void Ping();            // ping server

    void Exit();            // server and client shutdown

    void Status();          // check server status

    void SetFrequency();    

    void SetPower();

    void GetFrequency();

    void GetPower();

    void SetFault();

    void ClearFault();

    void ListFault();       // recieve and print table of faults

protected:
    void SendStr();         // send stream to server

    // unordered_map makes PrintCommandKeys() print in right order
    typedef std::pair<int, std::function<void()>> p;
    std::unordered_map <std::string, std::pair<int, std::function<void()>>> comms {
        {"ping",    p(TAG_PING,     std::bind(&SharedMemoryClient::Ping, this))},
        {"exit",    p(TAG_EXIT,     std::bind(&SharedMemoryClient::Exit, this))},
        {"status",  p(TAG_STATUS,   std::bind(&SharedMemoryClient::Status, this))},
        {"setfreq", p(TAG_SETFREQ,  std::bind(&SharedMemoryClient::SetFrequency, this))},
        {"setpwr",  p(TAG_SETPOWER, std::bind(&SharedMemoryClient::SetPower, this))},
        {"getfreq", p(TAG_GETFREQ,  std::bind(&SharedMemoryClient::GetFrequency, this))},
        {"getpwr",  p(TAG_GETPOWER, std::bind(&SharedMemoryClient::GetPower, this))},
        {"setflt",  p(TAG_SETFAULT, std::bind(&SharedMemoryClient::SetFault, this))},
        {"clrflt",  p(TAG_CLRFAULT, std::bind(&SharedMemoryClient::ClearFault, this))},
        {"listflt", p(TAG_LISTFAULT,std::bind(&SharedMemoryClient::ListFault, this))},
        {"toserv",  p(TAG_STRTOSERV,std::bind(&SharedMemoryClient::SendStr, this))}
    };

    void PrintCommandKeys();

public:
    SharedMemoryClient(const char *name) : shm(SharedMemory(name)) {}

    void WorkLoop();

    void Stop() { working = false; }
};
