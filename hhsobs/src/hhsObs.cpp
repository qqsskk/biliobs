// hhsObs.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "hhsObs.h"
#include "util/dstr.h"


#ifdef __APPLE__
#define INPUT_AUDIO_SOURCE  "coreaudio_input_capture"
#define OUTPUT_AUDIO_SOURCE "coreaudio_output_capture"
#elif _WIN32
#define INPUT_AUDIO_SOURCE  "wasapi_input_capture"
#define OUTPUT_AUDIO_SOURCE "wasapi_output_capture"
#else
#define INPUT_AUDIO_SOURCE  "pulse_input_capture"
#define OUTPUT_AUDIO_SOURCE "pulse_output_capture"
#endif

static std::string currentLogFile;
static std::string lastLogFile;
static const char *run_hhs_obs_program_init = "run_hhs_obs_program_init";

hhsObsApp::hhsObsApp()
{

}

hhsObsApp::~hhsObsApp()
{

}
//     const char *renderer = "Direct3D 11";
//     return (astrcmpi(renderer, "Direct3D 11") == 0) ? "libobs - d3d11" : "libobs - opengl";

#define DEFAULT_LANG "en-US"

bool hhsObsApp::InitGlobalConfig(const std::string& strglobal)
{
    int errorcode = GlobalCfg_.Open(strglobal.c_str(), CONFIG_OPEN_ALWAYS);
    if (errorcode != CONFIG_SUCCESS) {
        return false;
    }
    locale_ = config_get_string(GlobalCfg_, "General", "Language");
    config_set_default_string(GlobalCfg_, "General", "Language", DEFAULT_LANG);
    config_set_default_uint(GlobalCfg_, "General", "MaxLogs", 10);
#if _WIN32
    config_set_default_string(GlobalCfg_, "Video", "Renderer", "Direct3D 11");
#else
    config_set_default_string(GlobalCfg_, "Video", "Renderer", "OpenGL");
#endif
    config_set_default_bool(GlobalCfg_, "BasicWindow", "PreviewEnabled", true);
    config_set_default_string(GlobalCfg_, "Basic", "Profile", "Untitled");
    config_set_default_string(GlobalCfg_, "Basic", "ProfileDir", "Untitled");
    config_set_default_string(GlobalCfg_, "Basic", "SceneCollection", "Untitled");
    config_set_default_string(GlobalCfg_, "Basic", "SceneCollectionFile", "Untitled");
}

bool hhsObsApp::LoadAllModule()
{
    //添加路径
    obs_load_all_modules();
    return true;
}

void LoadAllModule()
{
    obs_load_all_modules();
}
