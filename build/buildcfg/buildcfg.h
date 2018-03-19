#ifndef __BUILD_BUILDCFG_BUILDCFG_H__
#define __BUILD_BUILDCFG_BUILDCFG_H__

/*

1   �򿪱����̵�����Դ�ļ���ɾ��ԭ�еİ汾��Ϣ����version.rc��ӵ�����

2   �� build/buildcfg Ŀ¼�¶�Ӧ��buildcfg.h�ļ�
	�ҵ��ù�������λ�ã�����Ӣ�ﶨ�����������꣬�磺	

#ifdef BUILD_ETDBE // size : 20000
	#pragma comment(linker, "/base:0x4DA00000")   
	#pragma message("\t    ETDBE")
	#define VI_FILEDESCRIPTION	"Data Import"	//�ļ�����
	#define VI_INTERNALNAME		"dbe"			//�ڲ�����
	#define VI_ORIGINALFILENAME	"etdbe.dll"		//ԭʼDLL�ļ���
#endif

3.  �� ETDBE �������õ� Resources ҳ��"Preprocessor definitions"����ӱ����̶�Ӧ�ĺ꣬�� BUILD_ETDBE
	��ע����������(Debug/Release/...)��Ҫ����
	��"Additionalresource include directories"�м���Coding/includeĿ¼�����λ��
	�� "../../builder/buildcfg"
	
 */

// -------------------------------------------------------------------------

#ifdef _WINDOWS_
#error "include buildcfg.h before include all other files"
#endif

#ifdef BUILD_BILIOBS_EXE
#pragma message("\t    BILIOBS_EXE")
#define VI_FILEDESCRIPTION	"Biliobs ��ģ��"
#define VI_INTERNALNAME		"biliobs.exe"
#define VI_ORIGINALFILENAME	"biliobs.exe"
#endif



#ifndef VI_FILEDESCRIPTION
#pragma error("not defined")
#define VI_FILEDESCRIPTION	  "Biliobs Module"
#define VI_INTERNALNAME		  ""
#define VI_ORIGINALFILENAME	  ""
#endif 
#ifndef VI_PRODUCT_NAME
  #define VI_PRODUCT_NAME	"Biliobs"
#endif

#endif /* __BUILD_BUILDCFG_BUILDCFG_H__ */
