#include "stdafx.h"
#include "AddInNative.h"

#pragma warning(disable : 4996)

const WCHAR_T* GetClassNames()
{
#ifdef _FILE
	FILE* f = fopen("C:\\Junk\\dllmain.txt", "a+");
	fprintf(f, "GetClassNames\n");
	fclose(f);
#endif

	return ADDINCLASSNAME;
}

long GetClassObject(const WCHAR_T* wsName, IComponentBase** pInterface)
{
#ifdef _FILE
	FILE* f = fopen("C:\\Junk\\dllmain.txt", "a+");
	fprintf(f, "GetClassObject: %ls\n", wsName);
	fclose(f);
#endif

	if (wcsicmp(wsName, ADDINCLASSNAME) != 0)
		return 0;

	if (*pInterface == nullptr)
	{
		*pInterface = new CAddInNative;
		return 1;
	}

	return 0;
}

AppCapabilities SetPlatformCapabilities(const AppCapabilities capabilities)
{
	return eAppCapabilities1;
}

long DestroyObject(IComponentBase** pIntf)
{
#ifdef _FILE
	FILE* f = fopen("C:\\Junk\\dllmain.txt", "a+");
	fprintf(f, "DestroyObject\n");
	fclose(f);
#endif

	delete *pIntf;
	*pIntf = nullptr;
	return 0;
}