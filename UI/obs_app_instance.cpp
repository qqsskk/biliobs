#include "stdafx.h"
#include "obs_app_instance.h"
#include "obs_hotkey_manager.h"
#include "obs_paths_manager.h"
using namespace std;

static bool MakeObsDirs(){

	//´´½¨Ä¿Â¼
	wstring wsUserdata =  GetUserDataPath();
	if (!ForceDirectoryW(wsUserdata))
		return false;
}

bool ObsApp::InitApp()
{
	try {

		HotkeyManager::InitializeInstance();
		EnableMuteHotkeySupport();

		if (!mOBSInitGlobalConfig())
			throw "Failed to initialize global config";

		if (!mOBSInit())
			return false;

	}
	catch (const char *error) {
		blog(LOG_ERROR, "%s", error);
		return false;
	}
	return true;
}

bool ObsApp::mOBSInit()
{
	return true;
}

bool ObsApp::mOBSInitGlobalConfig()
{
	return true;
}

bool ObsApp::mOBSInitGlobalConfigDefaults()
{
	return true;
}
