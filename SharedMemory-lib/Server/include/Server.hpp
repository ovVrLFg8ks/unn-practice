#pragma once

#include "SharedMemoryCore.hpp"

#include <iostream>
#include <ctime>
#include <chrono>
#include <map>
#include <vector>
#include <functional>

class SharedMemoryServer {
protected:
    std::string status = "OK!";     // server/equipment status
    int32_t frequency = 1000;       // equipment freq
    int32_t minFrequency = 1000;    // 1000 for example 
    int32_t power = 0;              // equipment power

    bool working = true;            // WorkLoop() state
    int sleepMs = 333;              // WorkLoop() sleep in loop

    struct Fault {
        int faultCode;
        //char severity;
        std::chrono::system_clock::time_point time;

        Fault(int faultCode, std::chrono::system_clock::time_point time) : faultCode(faultCode), time(time) {};
    };
    
    std::vector<Fault> faults;
    //std::multiset<Fault> s;
    SharedMemory shm;

public:
    virtual void Ping();            // answer client

    virtual void Exit();            // exit WorkLoop()

    virtual void Status();          // give client server/equipment status

    virtual void SetFrequency();    // set frequency by client

    virtual void SetPower();        // set power by client

    virtual void GetFrequency();    // client gets frequency

    virtual void GetPower();        // client gets power

    virtual void SetFault();        // client sets fault

    virtual void ClearFault();      // client clears fault

    virtual void ListFaults();      // send faults table to client

protected:
    void RecvStr();         // test 

    // map of previous functions to execute in WorkLoop()
    std::map<int, std::function<void()>> comms {
        {TAG_PING,      std::bind(&SharedMemoryServer::Ping, this)},
        {TAG_EXIT,      std::bind(&SharedMemoryServer::Exit, this)},
        {TAG_STATUS,    std::bind(&SharedMemoryServer::Status, this)},
        {TAG_SETFREQ,   std::bind(&SharedMemoryServer::SetFrequency, this)},
        {TAG_SETPOWER,  std::bind(&SharedMemoryServer::SetPower, this)},
        {TAG_GETFREQ,   std::bind(&SharedMemoryServer::GetFrequency, this)},
        {TAG_GETPOWER,  std::bind(&SharedMemoryServer::GetPower, this)},
        {TAG_SETFAULT,  std::bind(&SharedMemoryServer::SetFault, this)},
        {TAG_CLRFAULT,  std::bind(&SharedMemoryServer::ClearFault, this)},
        {TAG_LISTFAULT, std::bind(&SharedMemoryServer::ListFaults, this)},
        {TAG_STRTOSERV, std::bind(&SharedMemoryServer::RecvStr, this)}
    };

public:
    SharedMemoryServer(const char *name) : shm(SharedMemory(name)) {
        //shm.SetState(SM_CLIENT);            // pass control to client
    }

    // processes client requests
    void WorkLoop();

    void Stop() { working = false; shm.SetState(SM_SERVER); }
};
