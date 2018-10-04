#include "stdafx.h"
#include "dllcom.h"

HINSTANCE hInstance;

extern "C" BOOL WINAPI DllMain(
	HINSTANCE hInstance, 
	DWORD dwReason, 
	LPVOID lpReserved)
{
#ifdef _FILE
	FILE* f;
	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
		f = fopen("C:\\Junk\\dllmain.txt", "a+");
		fprintf(f, "DLL_PROCESS_ATTACH\n");
		fclose(f);
		break;
	case DLL_PROCESS_DETACH:
		f = fopen("C:\\Junk\\dllmain.txt", "a+");
		fprintf(f, "DLL_PROCESS_DETACH\n");
		fclose(f);
		break;
	}
#endif

	::hInstance = hInstance;
	return _AtlModule.DllMain(dwReason, lpReserved); 
}
