#pragma once

// shared memory unique name 
const char MEMNAME_RC[24] = "/6y8xOGvSmo-WtV_XO16eJA";  // radio-config connection
const char MEMNAME_RF[24] = "/7y9xOGvSmo-WtV_XO16eJB";  // radio-fault connection


// who owns memory
constexpr bool SM_SERVER = 0;
constexpr bool SM_CLIENT = 1;

// shared tags
constexpr int TAG_PING = 1;         // get simple answer from server
constexpr int TAG_EXIT = 2;         // client and server shutdown
constexpr int TAG_STATUS = 3;       // get server status
constexpr int TAG_SETFREQ = 4;      // set server frequency
constexpr int TAG_SETPOWER = 5;     // set server power
constexpr int TAG_GETFREQ = 6;      // get server frequency
constexpr int TAG_GETPOWER = 7;     // get server power
constexpr int TAG_SETFAULT = 8;     // create new fault 
constexpr int TAG_CLRFAULT = 9;     // erase fault
constexpr int TAG_LISTFAULT = 10;   // get table of faults
constexpr int TAG_STRTOSERV = 99;   // stream to server (test) (not required for task)