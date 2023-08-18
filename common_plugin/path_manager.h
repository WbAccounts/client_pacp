#ifndef COMMON_PATHMANAGER_H_
#define COMMON_PATHMANAGER_H_

#include <string>

class PathManager {
public:
    PathManager(){};
    // 各文件路径
    static std::string get_install_path();
    static std::string get_log_path();
    static std::string get_defense_conf_path();
};

#endif /* COMMON_PATHMANAGER_H_ */
