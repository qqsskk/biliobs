#pragma once
#include "obs-app-api.hpp"

/*
*推流工具全局函数
*/

class hhsObsApp
{
public:
    hhsObsApp(int &argc, char **argv);
    ~hhsObsApp();
public:
    bool mInitApp();
    TextLookup textLookup_;
    inline const char* GetString(const char *lookupVal) const { return textLookup_.GetString(lookupVal); }
    inline config_t* GetGlobalConfig() const { return GlobalConfig_; }
    profiler_name_store_t* GetProfilerNameStore() const { return ProfilerNameStore_; }

    const char* mGetRenderModule() const;

    const char* InputAudioSource() const;
    const char* OutputAudioSource() const;
private:
    bool OBSInit();
    bool OBSInitGlobalConfig();
    bool OBSInitGlobalConfigDefaults();
private:
    ConfigFile GlobalConfig_;
    profiler_name_store_t* ProfilerNameStore_ = nullptr;
    std::string locale_;
};