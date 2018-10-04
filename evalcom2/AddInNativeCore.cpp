#include "stdafx.h"
#include "Base64.h"
#include "AddInNativeCore.h"

CAddInNativeCore::CAddInNativeCore() :
	m_defBase(nullptr),
	m_memMan(nullptr)
{
}

bool CAddInNativeCore::Load(tVariant * pvarRetValue, tVariant * paParams)
{
	switch (paParams[0].vt)
	{
	case VTYPE_PWSTR:
		CInterpreter::Load(Base64::Decode(paParams[0].pwstrVal, paParams[0].strLen));
		break;
	case VTYPE_PSTR:
		CInterpreter::Load(Base64::Decode(paParams[0].pstrVal, paParams[0].strLen));
		break;
	default:
		return false;
	}

	pvarRetValue->vt = VT_BOOL;
	pvarRetValue->bVal = true;

	return true;
}

bool CAddInNativeCore::SetValue(tVariant * pvarRetValue, tVariant * paParams)
{
	pvarRetValue->vt = VT_BOOL;
	pvarRetValue->bVal = true;

	switch (paParams[0].vt)
	{
	case VTYPE_I2:
	case VTYPE_I4:
	case VTYPE_UI1:
		CInterpreter::SetValue(paParams[0].lVal);
		break;
	case VTYPE_BOOL:
		CInterpreter::SetValue(paParams[0].bVal);
		break;
	case VTYPE_R4:
	case VTYPE_R8:
		CInterpreter::SetValue(paParams[0].dblVal);
		break;
	default:
#ifdef _FILE
		FILE* f = fopen("C:\\Junk\\SetValue.txt", "w+");
		fprintf(f, "Unsupported tVariant type %d\n", paParams[0].vt);
		fclose(f);
#endif
		pvarRetValue->bVal = false;
	}
	
	return pvarRetValue->bVal;
}

bool CAddInNativeCore::Execute(tVariant * pvarRetValue, tVariant * paParams)
{
	pvarRetValue->vt = VTYPE_R8;
	return CInterpreter::Execute(pvarRetValue->dblVal);
}

#pragma warning(disable : 4996)

bool CAddInNativeCore::Version(tVariant * pvarRetValue, tVariant * paParams)
{
#ifdef _WIN64
	wchar_t* version = L"Native x64";
#else
	wchar_t* version = L"Native";
#endif

	pvarRetValue->vt = VTYPE_PWSTR;
	pvarRetValue->wstrLen = wcslen(version);
	
	if (m_memMan->AllocMemory((void**)&pvarRetValue->pwstrVal, sizeof(wchar_t)*(pvarRetValue->wstrLen + 1)))
	{
		wcscpy(pvarRetValue->pwstrVal, version);
		return true;
	}
	
	return false;
}