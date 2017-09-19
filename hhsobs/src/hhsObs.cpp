// hhsObs.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "hhsObs.h"

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

static bool StartupOBS(const char *locale, profiler_name_store_t *store) {

    char path[512];
    if (GetUserDataPath(path, sizeof(path), "common/plugin_config") <= 0)
        return false;

    return obs_startup(locale, path, store);
}

hhsObsApp::hhsObsApp(int &argc, char **argv)
{

}

hhsObsApp::~hhsObsApp()
{

}

const char* hhsObsApp::mGetRenderModule() const
{
    const char *renderer = config_get_string(GlobalConfig_, "Video", "Renderer");
    return (astrcmpi(renderer, "Direct3D 11") == 0) ? "libobs - d3d11" : "libobs - opengl";
}

const char* hhsObsApp::InputAudioSource() const
{
    return INPUT_AUDIO_SOURCE;
}

const char* hhsObsApp::OutputAudioSource() const
{
    return OUTPUT_AUDIO_SOURCE;
}

bool hhsObsApp::OBSInit()
{
    ProfileScope("BiLiApp::mOBSInit");

    bool licenseAccepted = config_get_bool(GlobalConfig_, "General", "LicenseAccepted");
    if (!licenseAccepted) {
        config_set_bool(GlobalConfig_, "General", "LicenseAccepted", true);
        config_save(GlobalConfig_);
    }

    if (!StartupOBS(locale_.c_str(), GetProfilerNameStore()))
        return false;

    return true;
}

bool hhsObsApp::OBSInitGlobalConfig()
{
    char path[512];
    int len = GetUserDataPath(path, sizeof(path), "global.ini");
    if (len <= 0)
        return false;

    int errorcode = GlobalConfig_.Open(path, CONFIG_OPEN_ALWAYS);
    if (errorcode != CONFIG_SUCCESS) {
        return false;
    }

    return OBSInitGlobalConfigDefaults();
}

#define DEFAULT_LANG "en-US"

bool hhsObsApp::OBSInitGlobalConfigDefaults()
{
    config_set_default_string(GlobalConfig_, "General", "Language", DEFAULT_LANG);
    const char *lang = config_get_string(GlobalConfig_, "General", "Language");
    locale_ = lang;
    config_set_default_uint(GlobalConfig_, "General", "MaxLogs", 10);

#if _WIN32
    config_set_default_string(GlobalConfig_, "Video", "Renderer", "Direct3D 11");
#else
    config_set_default_string(GlobalConfig_, "Video", "Renderer", "OpenGL");
#endif

    config_set_default_bool(GlobalConfig_, "BasicWindow", "PreviewEnabled", true);
    config_set_default_string(GlobalConfig_, "Basic", "Profile", "Untitled");
    config_set_default_string(GlobalConfig_, "Basic", "ProfileDir", "Untitled");
    config_set_default_string(GlobalConfig_, "Basic", "SceneCollection", "Untitled");
    config_set_default_string(GlobalConfig_, "Basic", "SceneCollectionFile", "Untitled");

    return true;
}
