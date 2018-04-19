#include "stdafx.h"
#include "obs_hotkey_manager.h"
#include "obs_config_helper.h"
#include "obs_thread_worker.h"

struct MatchHotkeyUserDataHelper
{
	std::string hotkeyName;
	std::vector<obs_hotkey_id> result;
};

HotkeyManager* HotkeyManager::instance_ = nullptr;

void HotkeyManager::RawHotkeyCallback(void *data, obs_hotkey_id id, obs_hotkey_t *hotkey, bool pressed)
{
	IHotkeyCallback* callback = static_cast<IHotkeyCallback*>(data);

	switch (obs_hotkey_get_registerer_type(hotkey))
	{
	case OBS_HOTKEY_REGISTERER_FRONTEND:
		callback->InvokeFrontend(pressed);
		break;

	case OBS_HOTKEY_REGISTERER_SOURCE:
	{
		obs_weak_source_t* weakSrc = static_cast<obs_weak_source_t*>(obs_hotkey_get_registerer(hotkey));
		obs_source_t* src = obs_weak_source_get_source(weakSrc);
		if (src)
		{
			callback->InvokeSource(src, pressed);
			obs_source_release(src);
		}
		break;
	}

	case OBS_HOTKEY_REGISTERER_OUTPUT:
	case OBS_HOTKEY_REGISTERER_SERVICE:
	case OBS_HOTKEY_REGISTERER_ENCODER:
		break;
	}
}

static bool MatchHotkeyKeyName(void *data, obs_hotkey_id id, obs_hotkey_t *key)
{
	MatchHotkeyUserDataHelper* p = static_cast<MatchHotkeyUserDataHelper*>(data);
	if (p->hotkeyName == obs_hotkey_get_name(key))
		p->result.push_back(id);
	return true;
}

bool HotkeyManager::Register(const char* hotkeyName, PHotkeyCallback&& callback)
{
	callbacks_.erase(hotkeyName);
	auto x = callbacks_.insert(std::make_pair(std::string(hotkeyName), PHotkeyCallback()));
	if (x.second == true)
		x.first->second = std::move(callback);
	return x.second;
}

void HotkeyManager::InitializeInstance()
{
	if (!instance_)
		instance_ = new HotkeyManager();
}

void HotkeyManager::UninitializeInstance()
{
	if (instance_)
		delete instance_;
	instance_ = nullptr;
}

HotkeyManager* HotkeyManager::GetInstance()
{
	return instance_;
}



bool HotkeyManager::Unregister(const char* hotkeyName)
{

	//解除快捷键注册
	MatchHotkeyUserDataHelper param;
	param.hotkeyName = hotkeyName;
	obs_enum_hotkeys(MatchHotkeyKeyName, &param);

	for (obs_hotkey_id& x : param.result)
		obs_hotkey_unregister(x);

	//移除对应表里的
	HotkeyMap::iterator r = callbacks_.find(hotkeyName);
	callbacks_.erase(r);
	return r != callbacks_.end();
}

obs_hotkey_id HotkeyManager::RegisterSource(obs_source_t* src, const char* hotkeyName)
{
	auto x = callbacks_.find(hotkeyName);
	if (x != callbacks_.end())
	{
		obs_hotkey_id hotkeyId = obs_get_source_hotkey_id(src, hotkeyName);
		if (hotkeyId != ~obs_hotkey_id())
			obs_hotkey_unregister(hotkeyId);
		return obs_hotkey_register_source(src, hotkeyName, hotkeyName, &HotkeyManager::RawHotkeyCallback, static_cast<IHotkeyCallback*>(x->second.get()));
	}
	else
		return ~obs_hotkey_id();
}

bool HotkeyManager::UnregisterSource(obs_source_t* src, const char* hotkeyName)
{
	obs_hotkey_id hotkeyId = obs_get_source_hotkey_id(src, hotkeyName);
	if (hotkeyId == ~obs_hotkey_id())
		return false;
	else
	{
		obs_hotkey_unregister(hotkeyId);
		return true;
	}
}

bool HotkeyManager::UnregisterFrontend(const char* hotkeyName)
{
	obs_hotkey_id hotkeyId = obs_get_frontend_hotkey_id(hotkeyName);
	if (hotkeyId == ~obs_hotkey_id())
		return false;
	else
	{
		obs_hotkey_unregister(hotkeyId);
		return true;
	}
}

obs_hotkey_id HotkeyManager::RegisterFrontend(const char* hotkeyName)
{
	auto x = callbacks_.find(hotkeyName);
	if (x != callbacks_.end())
	{
		obs_hotkey_id hotkeyId = obs_get_frontend_hotkey_id(hotkeyName);
		if (hotkeyId != ~obs_hotkey_id())
			obs_hotkey_unregister(hotkeyId);
		return obs_hotkey_register_frontend(hotkeyName, hotkeyName, &HotkeyManager::RawHotkeyCallback, static_cast<IHotkeyCallback*>(x->second.get()));
	}
	else
		return ~obs_hotkey_id();
}


static void* ShowNotice(const char* icon, const char* text)
{
	//显示界面
	return 0;
}

class SourceMuteHotkeySinker
{
public:
	void Triggered(obs_source_t* src, bool pressed)
	{
		if (!pressed)
			return;

		bool ismuted = obs_source_muted(src);
		const char* sourceId = obs_source_get_id(src);
		ObsThreadWorker::TaskT showNoticeWidget;
		if (strcmp(sourceId, "wasapi_input_capture") == 0)
		{
			if (ismuted)
				showNoticeWidget = std::bind(ShowNotice, ":/HotkeyTriggeredNotice/mic-unmute", "microphone unmuted");
			else
				showNoticeWidget = std::bind(ShowNotice, ":/HotkeyTriggeredNotice/mic-mute", "microphone muted");
		}
		else if (strcmp(sourceId, "wasapi_output_capture") == 0)
		{
			if (ismuted)
				showNoticeWidget = std::bind(ShowNotice, ":/HotkeyTriggeredNotice/system-unmute", "system sound unmuted");
			else
				showNoticeWidget = std::bind(ShowNotice, ":/HotkeyTriggeredNotice/system-mute", "system sound muted");
		}

		assert(static_cast<bool>(showNoticeWidget));
		obs_source_set_muted(src, !ismuted);
	}
};

void EnableMuteHotkeySupport()
{
	HotkeyManager::GetInstance()->Register("obs.mute-source", CreateSourceHotkeyCallback((SourceMuteHotkeySinker*)0, &SourceMuteHotkeySinker::Triggered));
}