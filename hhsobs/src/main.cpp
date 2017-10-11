#include "stdafx.h"
#include "hhsObs.h"

#include "base/command_line.h"

#ifdef WIN32
#include <Windows.h>
static const wchar_t* InstanceMutexName = L"E82991C9-BFF7-4F93-B6FE-E0DDDDDEWSWSS";
#endif

void obs_enum_module_callback(void *param, obs_module_t *module){
    int a = 0;
}

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPTSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    const char* argv = GetCommandLineA();
    base::CommandLine::Init(nCmdShow, &argv);
    base::CommandLine* cmd_line = base::CommandLine::ForCurrentProcess();
     
#ifdef WIN32
    HANDLE instanceMutex = CreateMutexW(NULL, TRUE, InstanceMutexName);
    if (GetLastError() == ERROR_ALREADY_EXISTS)
    {
        CloseHandle(instanceMutex);
        return 0;
    }
#endif

    std::string strGlobal;
    hhsObsApp app;
    app.InitGlobalConfig(strGlobal);
    
    profiler_name_store_t* mProfilerNameStore = nullptr;
    obs_startup("en-US", _W2A_(GetPath_Plugins()).c_str(), mProfilerNameStore);
    obs_load_all_modules();
    obs_enum_modules(obs_enum_module_callback, NULL);

    return 0;
}