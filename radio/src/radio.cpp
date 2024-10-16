#include "daemon.hpp"
using namespace daemonpp;

class radio : public daemon
{
public:
    void on_start(const dconfig& cfg) override {
      /// Runs once after daemon starts:
      /// Initialize your code here...

      dlog::info("on_start: radio version " + cfg.get("version") + " started!");
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