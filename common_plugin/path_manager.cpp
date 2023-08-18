#include "path_manager.h"

std::string PathManager::get_install_path() {
    //  TODO：路径位置待完善，需要添加路径的检测，读取运行路径的判断
    return std::string("./");
}

std::string PathManager::get_log_path() {
    return get_install_path() + "Log/pacp.log";
}

std::string PathManager::get_defense_conf_path() {
    return get_install_path() + "conf/defense.conf";
}