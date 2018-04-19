#pragma once

const std::wstring& GetUserDataPath();
const std::wstring& GetRootPath();
const std::wstring& GetDmpPath();

std::wstring trimW(const std::wstring &str);//去除两端的空格以及不可见字符
std::wstring ExcludeTrailingPathDelimiterW(const std::wstring &path);//去除文件路径末端的//
std::wstring IncludeTrailingPathDelimiterW(const std::wstring &path);//在文件路径末端的加上//
std::wstring ExtractFilePathW(const std::wstring &filestr, bool IncludeBackslash);//从文件路径中分离出文件所在的目录
std::wstring ExtractFileNameW(const std::wstring &filestr);//从文件路径中分离出文件名
std::wstring ExtractUrlNameW(const std::wstring &urlstr); //从url中分离文件名
std::wstring ExtractExtensionsW(const std::wstring &filestr, bool IncludePoint = true);
bool FileExistsW(const std::wstring &fn);//判断文件是否存在
bool DirectoryExistsW(const std::wstring &fn);//判断文件夹路径是否存在
bool ForceDirectoryW(const std::wstring &fn);//强制文件路径(不存在会递归创建)

//常用目录获取
std::wstring GetAppDataPath();//获取系统Appdata路径变量
std::wstring GetDesktopPath();//获取系统当前用户桌面路径变量
std::wstring GetAllUserDesktopPath();//获取系统全部用户桌面路径
std::wstring GetQuickLaunckPath();//获取系统快速启动栏路径
std::wstring GeTaskbarPath();//获取任务栏路径


int GetUserDataPath(char *path, size_t size, const char *name);