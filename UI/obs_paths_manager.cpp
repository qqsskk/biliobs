#include "stdafx.h"
#include "obs_paths_manager.h"
#include <Shlwapi.h>
#include <Shlobj.h>
#pragma comment(lib, "shlwapi.lib")

using namespace std;

#define kAPPDATA_FLODER_NAME             L"obs"
#define kAPPDATA_USERDATA_FLODER_NAME    L"obs\\userdata"
#define DMP_FLODER_NAME                  L"dmp"

class PathCache{
public:
	static PathCache& Instance(){
		static PathCache a;
		return a;
	}

	const std::wstring& GetUserDataPath(){
		return _userdata_path;
	}
	const std::wstring& GetRootPath(){
		return _root_path;
	}
	const std::wstring& GetDmpPath(){
		return _dmp_path;
	}
private:
	PathCache(){
		init();
	}
	void init(){
		wchar_t swzPath[MAX_PATH] = { 0 };
		SHGetFolderPathW(NULL, CSIDL_LOCAL_APPDATA, NULL, SHGFP_TYPE_CURRENT, swzPath);

		std::wstring appdata_root = swzPath;
		appdata_root.append(L"\\" kAPPDATA_FLODER_NAME L"\\");
		CreateDirectoryW(appdata_root.c_str(), NULL);

		_userdata_path = swzPath;
		_userdata_path.append(L"\\" kAPPDATA_USERDATA_FLODER_NAME L"\\");
		CreateDirectoryW(_userdata_path.c_str(), NULL);

		_dmp_path = _userdata_path;
		_dmp_path.append(DMP_FLODER_NAME L"\\");
		CreateDirectoryW(_dmp_path.c_str(), NULL);

		memset(swzPath, 0, sizeof(swzPath));
		GetModuleFileNameW(NULL, swzPath, 260 - 1);
		size_t off = _root_path.rfind('\\');
		if (std::wstring::npos != off){
			_root_path.resize(off);
		}
		_userdata_path.append(L"\\");
		_dmp_path.append(L"\\");
	}

private:
	std::wstring _userdata_path;
	std::wstring _root_path;
	std::wstring _dmp_path;
};

const std::wstring& GetRootPath(){
	return PathCache::Instance().GetRootPath();
}


const std::wstring& GetUserDataPath(){
	return PathCache::Instance().GetUserDataPath();
}

const std::wstring& GetDmpPath()
{
	//ForceDirectoryW(DMP_FLODER_NAME);
	return PathCache::Instance().GetDmpPath();
}

int GetUserDataPath(char *path, size_t size, const char *name)
{
	std::wstring wsPath = GetAppDataPath();
	os_wcs_to_utf8(wsPath.c_str(), wsPath.length(), path, size);
	errno_t err;
	err = strcat_s(path, size, name);
	if (err != 0)
		return -1;

	return strlen(path);
}

std::wstring trimW(const std::wstring &str)
{
	if (str.empty())
		return str;
	std::string::size_type i = -1, j = -1;
	UINT k;
	for (k = 0; k < str.length(); ++k)
	{
		if (unsigned(str[k]) > unsigned(L' '))
		{
			i = k;
			break;
		}
	}
	for (k = str.length() - 1; k >= 0; --k)
	{
		if (unsigned(str[k]) > unsigned(L' '))
		{
			j = k;
			break;
		}
	}
	i = (i == -1 ? 0 : i);
	j = (j == -1 ? str.length() - 1 : j);
	return str.substr(i, j - i + 1);
}

std::wstring ExcludeTrailingPathDelimiterW(const std::wstring &path)
{
	std::wstring s = trimW(path);
	if (s.empty())
		return s;
	if (s[s.length() - 1] == L'\\')
		return s.substr(0, s.length() - 1);
	else
		return s;
}

std::wstring IncludeTrailingPathDelimiterW(const std::wstring &path)
{
	std::wstring s = trimW(path);
	if (s.empty())
		return s;
	if (s[s.length() - 1] != L'\\')
		return s + L"\\";
	else
		return s;
}

std::wstring ExtractFilePathW(const std::wstring &filestr, bool IncludeBackslash)
{
	if (filestr.empty())
		return L"";
	for (int i = filestr.length() - 1; i >= 0; --i)
	{
		if (filestr[i] == L'\\')
		{
			if (IncludeBackslash)
				return filestr.substr(0, i + 1);
			else
				return filestr.substr(0, i);
		}
	}
	return L"";
}

std::wstring ExtractFileNameW(const std::wstring &filestr)
{
	if (filestr.empty())
		return L"";
	for (int i = filestr.length() - 1; i >= 0; --i)
	{
		if (filestr[i] == L'\\')
		{
			return filestr.substr(i + 1);
		}
	}
	return L"";
}

std::wstring ExtractUrlNameW(const std::wstring &urlstr)
{
	if (urlstr.empty())
		return L"";
	for (int i = urlstr.length() - 1; i >= 0; --i)
	{
		if (urlstr[i] == L'/')
		{
			return urlstr.substr(i + 1);
		}
	}
	return L"";
}

std::wstring ExtractExtensionsW(const std::wstring &filestr, bool IncludePoint /*= true*/)
{
	if (filestr.empty())
		return L"";
	for (int i = filestr.length() - 1; i >= 0; --i)
	{
		if (filestr[i] == L'.')
		{
			if (IncludePoint)
			{
				return filestr.substr(i);
			}
			else
			{
				return filestr.substr(i + 1);
			}
		}
	}
	return L"";
}

bool FileExistsW(const std::wstring &fn)
{
	DWORD Code = GetFileAttributesW(fn.c_str());
	if (Code == INVALID_FILE_ATTRIBUTES)
	{
		return false;
	}
	else
	{
		if ((Code & FILE_ATTRIBUTE_DIRECTORY) == 0)
		{
			return true;
		}
		else
		{
			return false;
		}

	}
}

bool DirectoryExistsW(const std::wstring &fn)
{
	DWORD Code = GetFileAttributesW(fn.c_str());
	return (Code != INVALID_FILE_ATTRIBUTES) && ((FILE_ATTRIBUTE_DIRECTORY & Code) != 0);
}

bool ForceDirectoryW(const std::wstring &fn)
{
	bool ret = true;
	if (fn.empty())
		return false;
	std::wstring sdir = ExcludeTrailingPathDelimiterW(fn);
	if (sdir.length() < 3 || DirectoryExistsW(sdir) || (ExtractFilePathW(sdir, true) == sdir))
		return ret;
	return ForceDirectoryW(ExtractFilePathW(sdir, true)) && CreateDirectoryW(sdir.c_str(), NULL);
}


std::wstring GetAppDataPath()
{
	wchar_t wcPath[MAX_PATH] = { 0 };
	if (SHGetSpecialFolderPathW(NULL, wcPath, CSIDL_APPDATA, FALSE) == TRUE){
		return wstring(wcPath);
	}
	return L"";
}

std::wstring GetDesktopPath()
{
	wchar_t sDesktopPath[MAX_PATH] = { 0 };
	if (SHGetSpecialFolderPath(NULL, sDesktopPath, CSIDL_DESKTOPDIRECTORY, FALSE) == TRUE) {
		return wstring(sDesktopPath);
	}
	return L"";
}

std::wstring GetAllUserDesktopPath()
{
	wchar_t sDesktopPath[MAX_PATH] = { 0 };
	if (SHGetSpecialFolderPath(NULL, sDesktopPath, CSIDL_COMMON_DESKTOPDIRECTORY, FALSE) == TRUE) {
		return wstring(sDesktopPath);
	}
	return L"";
}

std::wstring GetQuickLaunckPath()
{
	std::wstring csPath = GetAppDataPath();
	csPath += (L"\\Microsoft\\Internet Explorer\\Quick Launch");
	return csPath;
}

std::wstring GeTaskbarPath()
{
	std::wstring csPath = GetQuickLaunckPath();
	csPath += (L"\\User Pinned\\TaskBar");
	return csPath;
}
