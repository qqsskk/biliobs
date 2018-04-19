
#include "stdafx.h"

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	return 0;
}


LONG AppCrashHandleCallback(EXCEPTION_POINTERS *pE){

	//提示程序挂了，并上传dump
	return EXCEPTION_EXECUTE_HANDLER;
}
