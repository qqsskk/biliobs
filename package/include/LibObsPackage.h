#ifndef _LIB_OBS_PACKAGE_H_
#define _LIB_OBS_PACKAGE_H_

/*
*libobs进行二次封装并处理
*/

#ifdef LIBOBSPACK_EXPORTS
#define LIBOBSPACK_API __declspec(dllexport)
#else
#define LIBOBSPACK_API __declspec(dllimport)
#endif


class LibOBSPackage
{
public:
	LibOBSPackage();
	virtual ~LibOBSPackage();
};

#endif