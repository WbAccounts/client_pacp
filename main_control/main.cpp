#include <errno.h>
#include <malloc.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/file.h>

#include "common_plugin/path_manager.h"
#include "common_plugin/signal_helper.h"
#include "common/log/log.hpp"
#include "main_control/my_pcap.h"

static void set_fd_cloexec(int fd) {
    int flags = 0;
    flags     = fcntl(fd, F_GETFD);
    if (flags >= 0) {
        flags |= FD_CLOEXEC;
        fcntl(fd, F_SETFD, flags);
    }
}

static int runSingleton() {
    int         fdrunone = -1;
    std::string strpid   = PathManager::get_install_path() + "pcap.pid";
    fdrunone             = open(strpid.c_str(), O_RDWR | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    if (fdrunone == -1) {
        printf("%d %s open file failed. filename=%s\n", __LINE__, __FUNCTION__, strpid.c_str());
        return 1;
    }
    if (-1 == flock(fdrunone, LOCK_NB | LOCK_EX)) {
        printf("%d %s lock file failed\n", __LINE__, __FUNCTION__);
        return 1;
    }
    set_fd_cloexec(fdrunone);
    return 0;
}

// 后续根据传入参数添加无界面版本
// TODO：需要添加管理者进程，负责调起后台及界面进程，并保持与界面及后台的心跳检查，如果有一个进程终端，负责拉起进程（后面看有没有必要）
int main (int argc, char** argv) {

    // 设置超过1kb内存阈值后，使用mmap系统调用来分配内存
    mallopt(M_MMAP_THRESHOLD, 1 * 1024);  
    if (runSingleton()) {
        printf("PCAP is running...\n");
        return 0;
    }

    // TODO：后期添加日志配置文件，需要优化日志打印
    set_log_path(PathManager::get_log_path().c_str());

    SignaleHelper::install_signal_handler();
    sleep(1);  //等待程序大部分逻辑都运行起来

    sigset_t sigset;
    sigemptyset(&sigset);
    signal(SIGPIPE, SIG_IGN);
    sigaddset(&sigset, SIGALRM);
    sigaddset(&sigset, SIGIO);
    sigaddset(&sigset, SIGINT);
    sigaddset(&sigset, SIGTERM);
    sigaddset(&sigset, SIGHUP);
    sigaddset(&sigset, SIGWINCH);
    sigaddset(&sigset, SIGABRT);
    signal(SIGSEGV, qaxtrace::signal_backtrace);

    sigset_t emptysigset;
    sigemptyset(&emptysigset);

    if (sigprocmask(SIG_BLOCK, &sigset, NULL) == -1) {
        LOG_ERROR("ukey_mian:sigprocmask() failed, error(%d:%s).", errno, strerror(errno));
    }

    CPACP *pacp_backend = new CPACP();
    if (!pacp_backend->init()) {
        LOG_ERROR("Backend pcap init failed");
        _exit(0);
    }

    // TODO：crash 崩溃信息调试，这个后续要加上
    // std::string strCrashPath = login_utils::InitCrashDir(CRASH_DUMP_MAX);
    // crash_utils::Init(strCrashPath);

    while (!SignaleHelper::quit()) {sigsuspend(&emptysigset);}

    LOG_INFO("pcap_backend recv a quit signal!");
    LOG_INFO("pcap_backend exit!");
    exit(0);
}