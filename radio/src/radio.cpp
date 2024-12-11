#include "daemon.hpp"
#include "SharedMemory.hpp"
#include "Socket_Transport.h"
#include "NamedPipeLibrary.hpp"

#include <thread>
#include <string>

using namespace daemonpp;

class SharedMemoryServer_A : public SharedMemoryServer {
private:
  void Ping() {
    dlog::info("ping-pong");
    shm.SendStreamToClient((char*)"pong");
  }
public:
  void WorkLoop() {
      working = true;
      shm.SetState(SM_CLIENT);   
      while (working) {
          if (shm.GetState() == SM_SERVER) {
              if (comms.count(shm.GetTag()) > 0)
                  comms[shm.GetTag()]();
              else {
                  std::cout << "bad tag" << std::endl;
                  shm.SetState(SM_CLIENT);
              }
          }
          usleep(sleepMs * 1000);
      }
  }
  
  SharedMemoryServer_A(const char *name) : SharedMemoryServer(name) {
    dlog::info("started " + (std::string)shm.GetMemname() + " with err code " + std::to_string(shm.err));
  }
};

void ServLoop(SharedMemoryServer_A &server) {
    server.WorkLoop();  // process client requests
}
void RunClient(Transport& transport_soket) {
    transport_soket.Run();
}
void RunClientApp(ClientApp& client_app) {
    client_app.run();
}
class radio : public daemon
{
public:
    SharedMemoryServer_A server_RC = SharedMemoryServer_A(MEMNAME_RC);
    SharedMemoryServer_A server_RF = SharedMemoryServer_A(MEMNAME_RF);

    std::thread serveloop_RC;
    std::thread serveloop_RF;

    Transport Transp = Transport();

    std::thread client_thread;
    

    ClientApp clientApp = ClientApp("/tmp/fifo_request", "/tmp/fifo_response");
    std::thread clientAppThread = std::thread(RunClientApp, std::ref(clientApp));

    void on_start(const dconfig& cfg) override {
      /// Runs once after daemon starts:
      /// Initialize your code here...
      
      dlog::info("on_start: radio version " + cfg.get("version") + " started!");

      clientApp.run();
      Transp.Run();
      
      namedPipeThread = std::thread(RunNamedPipeClient, std::ref(pipeTransport));

      serveloop_RC = std::thread(ServLoop, std::ref(server_RC));
      serveloop_RF = std::thread(ServLoop, std::ref(server_RF));
    }

    void on_update() override {
      /// Runs every DURATION set in set_update_duration()...
      /// Update your code here...

      dlog::info("Hello, radio!");
    }

    void on_stop() override {
      /// Runs once before daemon is about to exit.
      /// Cleanup your code here...

      dlog::info("on_stop: radio stopped.");
      server_RC.Stop();
      server_RF.Stop();
      
      serveloop_RC.join();
      serveloop_RF.join();

      Transp.Stop_Socket();
      client_thread.join();

      clientAppThread.join();
    }

    void on_reload(const dconfig& cfg) override {
      /// Runs once after your daemon is reloaded
      /// Runs once after your daemon's config or service files are updated then reloaded with `$ systemctl reload my_daemon`

      dlog::info("on_reload: radio reloaded: " + cfg.get("version"));
    }
};


int main(int argc, const char* argv[]) {
  radio dmn;
  dmn.set_name("radio");
  dmn.set_update_duration(std::chrono::minutes(1));
  dmn.set_cwd("/");
  dmn.run(argc, argv);
  return 0;
}