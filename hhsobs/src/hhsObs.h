#pragma once
#include "util/util.hpp"
/*
*推流工具全局函数
*/

class hhsObsApp
{
public:
    hhsObsApp();
    ~hhsObsApp();
public:
    bool InitGlobalConfig(const std::string& strglobal);
    bool LoadAllModule();
private:
    ConfigFile GlobalCfg_;//全局配置文件
    profiler_name_store_t* ProfilerNameStore_ = nullptr;
    std::string locale_;
};

//加载全部模块
void LoadAllModule();