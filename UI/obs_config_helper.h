#pragma once

class ObsConfigFile
{
public:
	static bool SaveSceneData(obs_data_t* data);
	static obs_data_t* LoadSceneData();

	static bool SaveAudioDeviceConfig(obs_data_t* data);
	static obs_data_t* LoadAudioDeviceConfig();

	static bool SaveFrontendHotkeys(obs_data_t* data);
	static obs_data_t* LoadFrontendHotkeys();
};

//≥°æ∞≈‰÷√
class ObsSceneConfig
{
public:
	//≤ª∞¸∫¨order£°
	static obs_data_t* Get();
	static void Set(obs_data_t* data);
};

class ObsAudioDeviceConfig
{
public:
	static obs_data_t* Get();
	static void Set(obs_data_t* data);

	static void ResetAudioDevice(const char *sourceId, const char *deviceId, const char *deviceDesc, int channel);

	static void CreateFirstRunSources();


public:
	static void checkHasAudioDevicesFirst();
	static bool hasAudioInputDevice() { if (!checked_) checkHasAudioDevicesFirst();  return has_input_audio_device_; }
	static bool hasDesktopAudioDevice() { if (!checked_) checkHasAudioDevicesFirst(); return has_desktop_audio_device_; }
private:
	static bool checked_;
	static bool has_desktop_audio_device_;
	static bool has_input_audio_device_;

private:
	static bool HasAudioDevices(const char *source_id);

	static void initAudioInputDevice();
	static void initDesktopAudioDevice();
};


obs_hotkey_id obs_get_source_hotkey_id(obs_source_t* source, const char* hotkeyName);
obs_hotkey_id obs_get_frontend_hotkey_id(const char* hotkeyName);