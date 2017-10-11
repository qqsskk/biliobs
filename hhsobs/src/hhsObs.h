#pragma once
#include "util/util.hpp"
/*
*��������ȫ�ֺ���
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
    ConfigFile GlobalCfg_;//ȫ�������ļ�
    profiler_name_store_t* ProfilerNameStore_ = nullptr;
    std::string locale_;
};

//����ȫ��ģ��
void LoadAllModule();