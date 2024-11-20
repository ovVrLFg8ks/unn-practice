#include "daemon.hpp"
#include "SharedMemory.hpp"

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
                usleep(10000*1000);
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

class configuration : public daemon
{
public:
    SharedMemoryClient_A radioSM = SharedMemoryClient_A(MEMNAME_RC);
    std::thread loop_radioSM = std::thread(ClientLoop, std::ref(radioSM));

    void on_start(const dconfig& cfg) override {
      /// Runs once after daemon starts:
      /// Initialize your code here...

      dlog::info("on_start: configuration version " + cfg.get("version") + " started!");
    }

    void on_update() override {
      /// Runs every DURATION set in set_update_duration()...
      /// Update your code here...

      dlog::info("Hello, configuration!");
    }

    void on_stop() override {
      /// Runs once before daemon is about to exit.
      /// Cleanup your code here...

      radioSM.Stop();
      loop_radioSM.join();

      dlog::info("on_stop: configuration stopped.");
    }

    void on_reload(const dconfig& cfg) override {
      /// Runs once after your daemon is reloaded
      /// Runs once after your daemon's config or service files are updated then reloaded with `$ systemctl reload my_daemon`

      dlog::info("on_reload: configuration reloaded: " + cfg.get("version"));
    }
};


int main(int argc, const char* argv[]) {
  configuration dmn;
  dmn.set_name("configuration");
  dmn.set_update_duration(std::chrono::minutes(1));
  dmn.set_cwd("/");
  dmn.run(argc, argv);
  return 0;
}