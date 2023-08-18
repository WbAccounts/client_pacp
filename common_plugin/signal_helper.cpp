#include <execinfo.h>
#include <stdlib.h>
#include <unistd.h>
#include <syslog.h>

#include "signal_helper.h"
#include "common/log/log.hpp"
#include "common_plugin/path_manager.h"

sig_atomic_t SignaleHelper::m_quit = 0;

namespace qaxtrace {
    void signal_backtrace(int sig)
    {
        (void)sig;
        void *array[100];
        int size, i;
        char **strings;
        size = backtrace(array, 100);
        strings = backtrace_symbols(array, size);
        openlog(NULL, LOG_CONS | LOG_PID, LOG_DAEMON);
        for (i = 0; i < size; i++) {
            syslog(LOG_ERR, "%d: %s\n", i, strings[i]);
        }
        closelog();
        free(strings);
        strings = NULL;
        _exit(-1);
    }
}

void SignaleHelper::install_signal_handler() {
    struct sigaction act;
    // clear block signal
    sigemptyset(&act.sa_mask);
    // set transist extra param to handler function
    act.sa_flags = SA_SIGINFO;
    // set signal handler function
    act.sa_sigaction = SignaleHelper::exit_signal_handler;

    if (sigaction(SIGTERM, &act, NULL) < 0) {
        LOG_ERROR("install SIGTERM signal handler failed!");
    }
    if (sigaction(SIGABRT, &act, NULL) < 0) {
        LOG_ERROR("install SIGABRT signal handler failed!");
    }
    if (sigaction(SIGINT, &act, NULL) < 0) {
        LOG_ERROR("install SIGINT signal handler failed!");
    }
    if (sigaction(SIGQUIT, &act, NULL) < 0) {
        LOG_ERROR("install SIGQUIT signal handler failed!");
    }
    if (sigaction(SIGSEGV, &act, NULL) < 0) {
        LOG_ERROR("install SIGSEGV signal handler failed!");
    }
}

void SignaleHelper::exit_signal_handler(int n, siginfo_t *siginfo, void *myact){
    LOG_INFO("recv sig = [%d] from pid = [%d].", n, siginfo->si_pid);
    int sig = siginfo->si_signo;
    if (sig == SIGINT || sig == SIGTERM || sig == SIGQUIT || sig == SIGABRT || sig == SIGSEGV) {
        if (sig == SIGSEGV && siginfo->si_code > 0) { //si_code：内核态 >0，用户态 <=0
            qaxtrace::signal_backtrace(sig);
        }
        bool isDefenseAble = false;
        FILE *fp = fopen(PathManager::get_defense_conf_path().c_str(), "r"); // defense/switch 路径不确定 TODO
        if (fp) {
            char buff[32] = {0};
            fgets(buff, sizeof(buff), fp);
            fclose(fp);
            int nValue = atoi(buff);
            isDefenseAble = nValue == 0 ? false : true;
        }

        if (false == isDefenseAble // 没有开启自保功能
            || siginfo->si_pid == 1  //系统要求退出
            || siginfo->si_value.sival_int == 0x20000813) // 业务要求退出  sival_int只是信号携带的整数类型的值 TODO:这个值还没定，暂时用来在界面退出后台时使用
        {
            m_quit = 1;
        }
    }
}
