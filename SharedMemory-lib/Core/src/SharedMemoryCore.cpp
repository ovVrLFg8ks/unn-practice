#include "SharedMemoryCore.hpp"

int SharedMemory::Init() {
    bool exist = false;

    // create shared memory object
    fd = shm_open(memname, O_CREAT | O_EXCL | O_RDWR, S_IREAD | S_IWRITE | S_IEXEC);
    if (errno == EEXIST) {
        fd = shm_open(memname, O_RDWR, S_IREAD | S_IWRITE | S_IEXEC);
        exist = true;
    }
    else {
        if (ftruncate(fd, sizeof(shrData)) == -1)
            return -1;
    }
    if (fd == -1)
        return -2;

    // new mapping in the VAS
    shmp = (shrData *)mmap(NULL, sizeof(shrData), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (shmp == MAP_FAILED)
        return -3;

    if (!exist)
        shmp->state = SM_SERVER;
    return 0;
}

bool SharedMemory::CheckBroken() {
    if (std::filesystem::exists(mempath)) {
        struct stat st;
        stat(mempath.c_str(), &st);
        if (st.st_size == 0)
            return true;

    }
    return false;
}

void SharedMemory::CloseBroken() {
    if (CheckBroken()) {
        //err = -12;
        std::filesystem::remove(mempath, ec);
        //err = -13;
    }
}

int SharedMemory::Close() {
    if (munmap(shmp, sizeof(shrData)) == -1)
        return -1;

    shm_unlink(memname);

    if (close(fd) == -1)
        return -1;
    return 0;
}

void SharedMemory::int32ToChar(char a[], int32_t n) {
    memcpy(a, &n, sizeof(int32_t));
}

int32_t SharedMemory::charToInt32(char a[]) {
    int32_t n;
    memcpy(&n, a, sizeof(int32_t));
    return n;
}

void SharedMemory::SendStreamCore(size_t &dataLength, char *ar, bool sender) {
    for (size_t s = 0; s < dataLength; s += bufsize) {
        memcpy(&shmp->data, &ar[s], std::min(bufsize, dataLength - s));
        shmp->dataSize = dataLength - s;
        shmp->state = !sender;

        int i = 0;
        while (shmp->state != sender) {
            usleep(sleepDurationMs * 1000);
            if (i++ > awaitIter)
                return;
        }
    }
    shmp->state = !sender;
}

std::string SharedMemory::RecieveStreamCore(bool reciever) {
    std::string str = "";
    while (true) {
        int i = 0;
        while (shmp->state != reciever) {
            usleep(sleepDurationMs * 1000);
            if (i++ > awaitIter) {
                //shmp->state = reciever;
                str = "NO ANSWER";
                return str;
            }
        }
        str.append(shmp->data, std::min(bufsize, shmp->dataSize));
        shmp->state = !reciever;         // tell server you've done with it
        if (shmp->dataSize <= bufsize)
            break;
    }
    return str;
}

/*  server -> client  */
void SharedMemory::SendStreamToClient(std::string &str) {
    size_t dataLength = str.length();
    SendStreamCore(dataLength, str.data(), SM_SERVER);
}

void SharedMemory::SendStreamToClient(char *ar) {
    size_t dataLength = strlen(ar);
    SendStreamCore(dataLength, ar, SM_SERVER);
}

std::string SharedMemory::RecieveStreamFromServer() {
    return RecieveStreamCore(SM_CLIENT);
}

/*  client -> server  */
void SharedMemory::SendStreamToServer(std::string &str) {
    size_t dataLength = str.length();
    SendStreamCore(dataLength, str.data(), SM_CLIENT);
}

void SharedMemory::SendStreamToServer(char *ar) {
    size_t dataLength = strlen(ar);
    SendStreamCore(dataLength, ar, SM_CLIENT);
}

std::string SharedMemory::RecieveStreamFromClient() {
    return RecieveStreamCore(SM_SERVER);
}
