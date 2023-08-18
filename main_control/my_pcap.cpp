#include <iostream>
#include "main_control/my_pcap.h"
#include "common/AsFramework/ASBundleImpl.hpp"
#include "common/log/log.hpp"
#include "common_plugin/path_manager.h"

bool CPACP::init() {
    LOG_INFO("----->CPACP init start<----");
    if (!initLog()) {
        LOG_ERROR("%s init log failed", __FUNCTION__);
        return false;
    }
    if (!initSocketMgr()) {
        LOG_ERROR("%s init socket failed", __FUNCTION__);
        return false;
    }
    if (!initDlopen()) {
        LOG_ERROR("%s init dlopen failed", __FUNCTION__);
        return false;
    }
    LOG_INFO("----->CPACP init end<----");
    return true;
}

bool CPACP::initLog() {
    return true;
}

bool CPACP::initDlopen() {
    return true;
}

bool CPACP::initSocketMgr() {
    return true;
}