#include "daemon.hpp"
#include "SharedMemory.hpp"
#include "Socket_Transport.h"
#include "NamedPipeLibrary.hpp"

#include <thread>

using namespace daemonpp;

class SharedMemoryClient_A : public SharedMemoryClient {
private:
  void Ping() {
      shm.SetState(SM_SERVER);
      dlog::info(shm.RecieveStreamFromServer());
  }
public:
    void WorkLoop() {
        working = true;
        std::string command = "ping";    // user command (ex. "ping")
        while (working) {
            if (AwaitLoop() == -1) {
                dlog::info("NO ANS");
                usleep(1000*1000);
                continue;
            }
            shm.SetTag(comms[command].first);
            comms[command].second();
            usleep(10000*1000);
        }
    }

    SharedMemoryClient_A(const char *name) : SharedMemoryClient(name) {}
};

void ClientLoop(SharedMemoryClient_A &client) {
    client.WorkLoop();
}
void RunClient(Transport& transport_soket) {
    transport_soket.Run();
}

void RunNamedPipeServer(ServerApp &server) {
    server.run();
}

class fault : public daemon {

public:
    SharedMemoryClient_A radioSM = SharedMemoryClient_A(MEMNAME_RF);
    std::thread loop_radioSM;

    Transport Transp = Transport();
    std::thread client_thread;
    
    ServerApp server = ServerApp("/tmp/fifo_request", "/tmp/fifo_response");
    std::thread server_thread;

    void on_start(const dconfig& cfg) override {
      /// Runs once after daemon starts:
      /// Initialize your code here...

      server_thread = std::thread([this]() {
          RunNamedPipeServer();
      });

      dlog::info("on_start: fault version " + cfg.get("version") + " started!");

      client_thread = std::thread(RunClient, std::ref(Transp));
      Transp.Run();
      
      namedPipeThread = std::thread(RunNamedPipeServer, std::ref(pipeTransport));

      loop_radioSM = std::thread(ClientLoop, std::ref(radioSM));

      pipeTransport.Initialize("/tmp/named_pipe_transport");
    }

    void on_update() override {
      /// Runs every DURATION set in set_update_duration()...
      /// Update your code here...

      dlog::info("Hello, fault!");
    }

    void on_stop() override {
      /// Runs once before daemon is about to exit.
      /// Cleanup your code here...

      radioSM.Stop();
      Transp.Stop_Socket();

      loop_radioSM.join();
      if (server_thread.joinable()) {
          server_thread.join();
      }
      loop_radioSM.join();
      client_thread.join();
      
      dlog::info("on_stop: fault stopped.");
    }

    void on_reload(const dconfig& cfg) override {
      /// Runs once after your daemon is reloaded
      /// Runs once after your daemon's config or service files are updated then reloaded with `$ systemctl reload my_daemon`

      dlog::info("on_reload: fault reloaded: " + cfg.get("version"));
    }
};


int main(int argc, const char* argv[]) {
  fault dmn;
  dmn.set_name("fault");
  dmn.set_update_duration(std::chrono::minutes(1));
  dmn.set_cwd("/");
  dmn.run(argc, argv);
  return 0;
}
