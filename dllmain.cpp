// dllmain.cpp : Implementierung von DllMain.

#include "stdafx.h"
#include "resource.h"
#include "SampleProtocolHandler_i.h"
#include "dllmain.h"

CSampleProtocolHandlerModule _AtlModule;


extern HINSTANCE g_instanceDLL = 0;
// DLL-Einstiegspunkt
extern "C" BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	g_instanceDLL = hInstance;
	return _AtlModule.DllMain(dwReason, lpReserved); 
}
