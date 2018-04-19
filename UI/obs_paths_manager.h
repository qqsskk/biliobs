#pragma once

const std::wstring& GetUserDataPath();
const std::wstring& GetRootPath();
const std::wstring& GetDmpPath();

std::wstring trimW(const std::wstring &str);//ȥ�����˵Ŀո��Լ����ɼ��ַ�
std::wstring ExcludeTrailingPathDelimiterW(const std::wstring &path);//ȥ���ļ�·��ĩ�˵�//
std::wstring IncludeTrailingPathDelimiterW(const std::wstring &path);//���ļ�·��ĩ�˵ļ���//
std::wstring ExtractFilePathW(const std::wstring &filestr, bool IncludeBackslash);//���ļ�·���з�����ļ����ڵ�Ŀ¼
std::wstring ExtractFileNameW(const std::wstring &filestr);//���ļ�·���з�����ļ���
std::wstring ExtractUrlNameW(const std::wstring &urlstr); //��url�з����ļ���
std::wstring ExtractExtensionsW(const std::wstring &filestr, bool IncludePoint = true);
bool FileExistsW(const std::wstring &fn);//�ж��ļ��Ƿ����
bool DirectoryExistsW(const std::wstring &fn);//�ж��ļ���·���Ƿ����
bool ForceDirectoryW(const std::wstring &fn);//ǿ���ļ�·��(�����ڻ�ݹ鴴��)

//����Ŀ¼��ȡ
std::wstring GetAppDataPath();//��ȡϵͳAppdata·������
std::wstring GetDesktopPath();//��ȡϵͳ��ǰ�û�����·������
std::wstring GetAllUserDesktopPath();//��ȡϵͳȫ���û�����·��
std::wstring GetQuickLaunckPath();//��ȡϵͳ����������·��
std::wstring GeTaskbarPath();//��ȡ������·��


int GetUserDataPath(char *path, size_t size, const char *name);