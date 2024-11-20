#pragma once

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>          //  O_... definitions
#include <string.h>
#include <string>
#include <errno.h>
#include <cstring>
#include <algorithm>


#include "Protocol.hpp"

class SharedMemory {
private:
    // shared memory owner
    static constexpr bool SM_SERVER = 0;
    static constexpr bool SM_CLIENT = 1;

    const char *memname;                    // shared memory unique name
    constexpr static size_t bufsize = 16;   // minimum 4 to fit int32 
    u_int16_t sleepDurationMs = 100;        // sleep in loops
    int awaitIter = 10;                     // loops max number of iteratoins (in case of server not answering)
    int fd;                                 // shared memory object

    // shared memory data
    struct shrData {
        bool state = SM_SERVER;  // who owns memory (server/client)
        int tag = 0;            // specifies type of the current request
        size_t dataSize = 0;    // used for text stream only
        char data[bufsize] {0}; 
    };

    shrData *shmp;  // mapping in VAS

    int Init();     // create shared memory object and mappings

    int Close();    // close shared memory

    void int32ToChar(char a[], int32_t n); // place int32 to shrData.data[]

    int32_t charToInt32(char a[]);         // extract int32 from shrData.data[]

    void SendStreamCore(size_t &dataLength, char *ar, bool sender);

    std::string RecieveStreamCore(bool reciever);

public:

    SharedMemory(const char *memname) : memname(memname) {
        Init();
    }
    
    ~SharedMemory() {
        Close();
    }

    bool GetState() { return shmp->state; }

    void SetState(bool st) { shmp->state = st; }

    int GetTag() { return shmp->tag; }

    void SetTag(int tag) { shmp->tag = tag; }

    int32_t GetNumber() { return charToInt32(shmp->data); }

    void SetNumber(int32_t num) { int32ToChar(shmp->data, num); }

    /*
    in SendStream(..) and RecieveStream(..)
    data is sent in parts
    in case it's not fully fits in shrData.data[]
    */

    /*  server -> client  */
    void SendStreamToClient(std::string &str);

    void SendStreamToClient(char *ar);

    std::string RecieveStreamFromServer();

    /*  client -> server  */
    void SendStreamToServer(std::string &str);

    void SendStreamToServer(char *ar);

    std::string RecieveStreamFromClient();
};
