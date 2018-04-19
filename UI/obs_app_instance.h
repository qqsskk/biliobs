#pragma once
#pragma once

class ObsApp
{
public:
	ObsApp(int &argc, char **argv);
	~ObsApp();

	bool InitApp();

	TextLookup mTextLookup;
	inline const char *mGetString(const char *lookupVal) const { return mTextLookup.GetString(lookupVal); }
	inline config_t *mGetGlobalConfig() const { return mGlobalConfig; }
	profiler_name_store_t *mGetProfilerNameStore() const { return mProfilerNameStore; }
	const char *mGetRenderModule() const;

	const char* InputAudioSource() const;
	const char* OutputAudioSource() const;

	void mLoadTranslator();

private:
	ConfigFile mGlobalConfig;
	profiler_name_store_t          *mProfilerNameStore = nullptr;
	std::string mLocale;

	bool mOBSInit();
	bool mOBSInitGlobalConfig();
	bool mOBSInitGlobalConfigDefaults();

};