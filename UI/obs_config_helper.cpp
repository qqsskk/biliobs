#include "stdafx.h"
#include "obs_config_helper.h"
#include "obs_paths_manager.h"
#include "obs_source_helper.h"
#include "obs_hotkey_manager.h"
#include "obs_thread_worker.h"

#define BILIOBS_DIR "obs\\"
#define SCENE_CONFIGFILE "sceneConfig.json"
#define AUDIODEVICE_CONFIGFILE "audioDevices.json"
#define FRONTEND_HOTKEY_CONFIGFILE "frontendHotkeys.json"

const char* OBS_HOTKEY_MUTE = "obs.mute-source";
const char* OBS_HOTKEY_PUSHTOTALK = "obs.push-to-talk";


bool ObsConfigFile::SaveSceneData(obs_data_t* data)
{
	std::vector<char> savePathData(512);
	GetUserDataPath(&savePathData[0], savePathData.size(), SCENE_CONFIGFILE);
	return obs_data_save_json_safe(data, &savePathData[0], "tmp", "bak");
}

obs_data_t* ObsConfigFile::LoadSceneData()
{
	std::vector<char> savePathData(512);
	GetUserDataPath(&savePathData[0], savePathData.size(), SCENE_CONFIGFILE);
	return obs_data_create_from_json_file(&savePathData[0]);
}

bool ObsConfigFile::SaveAudioDeviceConfig(obs_data_t* data)
{
	std::vector<char> savePathData(512);
	GetUserDataPath(&savePathData[0], savePathData.size(), AUDIODEVICE_CONFIGFILE);
	return obs_data_save_json_safe(data, &savePathData[0], "tmp", "bak");
}

obs_data_t* ObsConfigFile::LoadAudioDeviceConfig()
{
	std::vector<char> savePathData(512);
	GetUserDataPath(&savePathData[0], savePathData.size(), AUDIODEVICE_CONFIGFILE);
	return obs_data_create_from_json_file(&savePathData[0]);
}

bool ObsConfigFile::SaveFrontendHotkeys(obs_data_t* data)
{
	std::vector<char> savePathData(512);
	GetUserDataPath(&savePathData[0], savePathData.size(), FRONTEND_HOTKEY_CONFIGFILE);
	return obs_data_save_json_safe(data, &savePathData[0], "tmp", "bak");
}

obs_data_t* ObsConfigFile::LoadFrontendHotkeys()
{
	std::vector<char> savePathData(512);
	GetUserDataPath(&savePathData[0], savePathData.size(), FRONTEND_HOTKEY_CONFIGFILE);
	return obs_data_create_from_json_file(&savePathData[0]);
}

obs_data_t* ObsSceneConfig::Get()
{
	obs_data_t*	      result;
	obs_data_array_t* sourceArray;
	obs_source_t*     currentScene;
	const char*       currentSceneName;

	result = obs_data_create();

	//保存所有来源，包括场景和场景元素
	sourceArray = obs_save_sources();

	//当前场景和当前场景名
	currentScene = obs_get_output_source(0);
	currentSceneName = obs_source_get_name(currentScene);

	obs_data_set_string(result, "currentScene", currentSceneName);
	obs_data_set_array(result, "sources", sourceArray);

	obs_data_array_release(sourceArray);
	obs_source_release(currentScene);

	return result;
}

void ObsSceneConfig::Set(obs_data_t* data)
{
	obs_data_array_t* sourceArray;
	const char*       currentSceneName;

	currentSceneName = obs_data_get_string(data, "currentScene");
	sourceArray = obs_data_get_array(data, "sources");

	obs_load_sources(sourceArray);

	//恢复“当前scene”
	for (OBSSource& src : OBSEnumSources())
	{
		const char* sourceName = obs_source_get_name(src);
		if (strcmp(currentSceneName, sourceName) == 0)
		{
			obs_set_output_source(0, src);
			break;
		}
	}
	obs_data_array_release(sourceArray);
}



struct FindSourceHotkeyData
{
	obs_hotkey_id id; //output
	obs_source_t* src;
	std::string hotkeyName;
};

static bool find_source_hotkey_proc(void *data, obs_hotkey_id id, obs_hotkey_t *key)
{
	FindSourceHotkeyData* p = static_cast<FindSourceHotkeyData*>(data);
	if (obs_hotkey_get_registerer_type(key) == OBS_HOTKEY_REGISTERER_SOURCE
		&& p->hotkeyName == obs_hotkey_get_name(key))
	{
		obs_weak_source_t* weakSrc = static_cast<obs_weak_source_t*>(obs_hotkey_get_registerer(key));
		obs_source_t* source = obs_weak_source_get_source(weakSrc);
		if (source)
		{
			if (source == p->src)
			{
				p->id = obs_hotkey_get_id(key);
				obs_source_release(source);
				return false;
			}
		}
	}
	return true;
}
struct FindFrontendHotkeyData
{
	obs_hotkey_id id; //output
	std::string hotkeyName;
};

obs_hotkey_id obs_get_source_hotkey_id(obs_source_t* source, const char* hotkeyName)
{
	FindSourceHotkeyData fshd;
	fshd.hotkeyName = hotkeyName;
	fshd.src = source;
	fshd.id = ~(obs_hotkey_id());
	obs_enum_hotkeys(find_source_hotkey_proc, &fshd);
	return fshd.id;
}

static bool find_frontend_hotkey_proc(void* data, obs_hotkey_id id, obs_hotkey_t* key)
{
	FindFrontendHotkeyData* p = static_cast<FindFrontendHotkeyData*>(data);
	if (obs_hotkey_get_registerer_type(key) == OBS_HOTKEY_REGISTERER_FRONTEND)
	{
		if (p->hotkeyName == obs_hotkey_get_name(key))
		{
			p->id = obs_hotkey_get_id(key);
			return false;
		}
	}
	return true;
}

obs_hotkey_id obs_get_frontend_hotkey_id(const char* hotkeyName)
{
	FindFrontendHotkeyData fshd;
	fshd.hotkeyName = hotkeyName;
	fshd.id = ~(obs_hotkey_id());
	obs_enum_hotkeys(find_frontend_hotkey_proc, &fshd);
	return fshd.id;
}

bool ObsAudioDeviceConfig::checked_ = false;
bool ObsAudioDeviceConfig::has_desktop_audio_device_;
bool ObsAudioDeviceConfig::has_input_audio_device_;

bool ObsAudioDeviceConfig::HasAudioDevices(const char *source_id)
{
	const char *output_id = source_id;
	obs_properties_t *props = obs_get_source_properties(
		OBS_SOURCE_TYPE_INPUT, output_id);
	size_t count = 0;

	if (!props)
		return false;

	obs_property_t *devices = obs_properties_get(props, "device_id");
	if (devices)
		count = obs_property_list_item_count(devices);

	obs_properties_destroy(props);

	return count != 0;
}

void ObsAudioDeviceConfig::initAudioInputDevice()
{
	//ResetAudioDevice(App()->mInputAudioSource(), "default",Str("AuxDevice1"), 3);
}

void ObsAudioDeviceConfig::initDesktopAudioDevice()
{
	//ResetAudioDevice(App()->mOutputAudioSource(), "default",Str("DesktopDevice1"), 1);
}

static struct {
	const char* deviceName;
	int deviceChannel;
} audioDeviceList[] =
{
	"DesktopAudioDevice1", 1,
	//	"DesktopAudioDevice2", 2,
	"AuxAudioDevice1", 3,
	//	"AuxAudioDevice2",     4,
	//	"AuxAudioDevice3",     5,
};

obs_data_t* ObsAudioDeviceConfig::Get()
{
	obs_data_t* result;
	result = obs_data_create();
	if (result)
	{
		int deviceCount = sizeof(audioDeviceList) / sizeof(*audioDeviceList);
		for (int i = 0; i < deviceCount; ++i)
		{
			obs_source_t* source = obs_get_output_source(audioDeviceList[i].deviceChannel);
			if (source)
			{
				obs_data_t* savedData = obs_save_source(source);
				obs_data_set_obj(result, audioDeviceList[i].deviceName, savedData);
				obs_source_release(source);
				obs_data_release(savedData);
			}
		}
	}
	return result;
}

void ObsAudioDeviceConfig::Set(obs_data_t* data)
{
	if (!data) {
		CreateFirstRunSources();
	}

	bool hasInputAudio = hasAudioInputDevice();
	bool hasDesktopAudio = hasDesktopAudioDevice();

// 	bool has_input_audio_data;
// 	bool has_desktop_audio_data;

	bool isNewSource = false;

	int deviceCount = sizeof(audioDeviceList) / sizeof(*audioDeviceList);
	for (int i = 0; i < deviceCount; ++i)
	{
		obs_source_t* outputSrc = obs_get_output_source(audioDeviceList[i].deviceChannel);
		if (outputSrc)
			isNewSource = false;
		else
			isNewSource = true;

		obs_data_t* savedData = obs_data_get_obj(data, audioDeviceList[i].deviceName);
		if (savedData)
		{
			if (1 == audioDeviceList[i].deviceChannel) {
				/*desktop audio data*/
				if (!hasDesktopAudio)
					goto SKIP;
			}
			else if (3 == audioDeviceList[i].deviceChannel) {
				/*input_audio data*/
				if (!hasInputAudio)
					goto SKIP;
			}

			if (isNewSource) //不存在，创建
			{
				obs_source_t* savedSource = obs_load_source(savedData);
				if (savedSource)
				{
					obs_set_output_source(audioDeviceList[i].deviceChannel, savedSource);
					obs_source_release(savedSource);
				}
			}
			else //存在，只更新
			{
				obs_data_t* settings = obs_data_get_obj(savedData, "settings");
				if (settings)
				{
					obs_source_update(outputSrc, settings);
					obs_data_release(settings);
				}
			}
		SKIP:
			obs_data_release(savedData);
		}
		else {
			if (1 == audioDeviceList[i].deviceChannel) {
				if (hasDesktopAudio)
				{
					initDesktopAudioDevice();
				}
			}
			else if (3 == audioDeviceList[i].deviceChannel) {
				if (hasInputAudio)
				{
					initAudioInputDevice();
				}
			}
		}

		obs_source_release(outputSrc);

		//添加快捷键支持
		if (isNewSource)
		{
			outputSrc = obs_get_output_source(audioDeviceList[i].deviceChannel);
			if (outputSrc)
			{
				HotkeyManager::GetInstance()->RegisterSource(outputSrc, OBS_HOTKEY_MUTE);
			}
		}
	}
}

void ObsAudioDeviceConfig::CreateFirstRunSources()
{
	if (hasDesktopAudioDevice())
		initDesktopAudioDevice();

	if (hasAudioInputDevice())
		initAudioInputDevice();
}

void ObsAudioDeviceConfig::checkHasAudioDevicesFirst()
{
	assert(!checked_);
	checked_ = true;

// 	has_input_audio_device_ = HasAudioDevices(App()->mInputAudioSource());
// 	has_desktop_audio_device_ = HasAudioDevices(App()->mOutputAudioSource());
}

