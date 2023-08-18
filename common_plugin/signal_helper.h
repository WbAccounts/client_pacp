#ifndef SIGNAL_HELPER
#define SIGNAL_HELPER

#include <signal.h>

namespace qaxtrace {
    void signal_backtrace(int sig);
}

class SignaleHelper {
  public:
    SignaleHelper() {}
    ~SignaleHelper(){}

  public:
    static void install_signal_handler();
    static void exit_signal_handler(int n, siginfo_t *siginfo, void *myact);
    static bool quit() { return (m_quit != 0); }

  private:
    static sig_atomic_t m_quit;
};

#endif