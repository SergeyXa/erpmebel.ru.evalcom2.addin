#include "stdafx.h"
#include <locale.h>
#include "AddInNative.h"

#pragma warning(disable: 4996)

//////////////////////////////////////////////////////////////////

namespace _native {

	typedef bool(CAddInNative::*MethodProc)(tVariant * paParams);
	typedef bool(CAddInNative::*MethodFunc)(tVariant * pvarRetValue, tVariant * paParams);

	enum MethodId {
		eMethodLoad,
		eMethodSetValue,
		eMethodExecute,
		eMethodVersion,
		eMethodLast
	};

	struct MethodInfo {
		MethodId id;
		wchar_t* name;
		wchar_t* nameRu;
		long nParams;
		bool hasRetVal;
		MethodProc pProc;
		MethodFunc pFunc;
	};

	MethodInfo methodList[eMethodLast] = {
		{ eMethodLoad,     L"Load",     L"Загрузить",          1, true, nullptr, &CAddInNative::Load },
		{ eMethodSetValue, L"SetValue", L"УстановитьЗначение", 1, true, nullptr, &CAddInNative::SetValue },
		{ eMethodExecute,  L"Execute",  L"Выполнить",          0, true, nullptr, &CAddInNative::Execute },
		{ eMethodVersion,  L"Version",  L"Версия",             0, true, nullptr, &CAddInNative::Version },
	};
}
using namespace _native;

//////////////////////////////////////////////////////////////////

bool ADDIN_API CAddInNative::Init(void * disp)
{
	m_defBase = (IAddInDefBase*)disp;
	return m_defBase != nullptr;
}

bool ADDIN_API CAddInNative::setMemManager(void * mem)
{
	m_memMan = (IMemoryManager*)mem;
	return m_memMan != nullptr;
}

long ADDIN_API CAddInNative::GetInfo()
{
	return 2000;
}

void ADDIN_API CAddInNative::Done()
{
}

bool ADDIN_API CAddInNative::RegisterExtensionAs(WCHAR_T ** wsExtensionName)
{
	if (m_memMan)
		if (m_memMan->AllocMemory((void**)wsExtensionName, (wcslen(ADDINCLASSNAME) + 1) * sizeof(WCHAR_T)))
		{
			wcscpy(*wsExtensionName, ADDINCLASSNAME);
			return true;
		}
	return false;
}

long ADDIN_API CAddInNative::GetNProps()
{
	return 0;
}

long ADDIN_API CAddInNative::FindProp(const WCHAR_T * wsPropName)
{
	return -1;
}

const WCHAR_T *ADDIN_API CAddInNative::GetPropName(long lPropNum, long lPropAlias)
{
	return nullptr;
}

bool ADDIN_API CAddInNative::GetPropVal(const long lPropNum, tVariant * pvarPropVal)
{
	return false;
}

bool ADDIN_API CAddInNative::SetPropVal(const long lPropNum, tVariant * varPropVal)
{
	return false;
}

bool ADDIN_API CAddInNative::IsPropReadable(const long lPropNum)
{
	return false;
}

bool ADDIN_API CAddInNative::IsPropWritable(const long lPropNum)
{
	return false;
}

long ADDIN_API CAddInNative::GetNMethods()
{
	return eMethodLast;
}

long ADDIN_API CAddInNative::FindMethod(const WCHAR_T * wsMethodName)
{
	long lMethodNum = -1;
	for (int i = 0; i < eMethodLast; ++i)
		if (wcsicmp(wsMethodName, methodList[i].name) == 0
			|| wcsicmp(wsMethodName, methodList[i].nameRu) == 0)
		{
			lMethodNum = i;
			break;
		}
	return lMethodNum;
}

const WCHAR_T *ADDIN_API CAddInNative::GetMethodName(const long lMethodNum, const long lMethodAlias)
{
	if (lMethodNum >= eMethodLast)
		return nullptr;

	wchar_t* methodName = (lMethodAlias == 0
		? methodList[lMethodNum].name
		: methodList[lMethodNum].nameRu);

	wchar_t* wsMethodName = nullptr;

	m_memMan->AllocMemory((void**)&wsMethodName, sizeof(wchar_t)*(wcslen(methodName) + 1));
	wcscpy(wsMethodName, methodName);

	return wsMethodName;
}

long ADDIN_API CAddInNative::GetNParams(const long lMethodNum)
{
	if (lMethodNum >= eMethodLast)
		return E_INVALIDARG;

	return methodList[lMethodNum].nParams;
}

bool ADDIN_API CAddInNative::GetParamDefValue(const long lMethodNum, const long lParamNum, tVariant * pvarParamDefValue)
{
	return false;
}

bool ADDIN_API CAddInNative::HasRetVal(const long lMethodNum)
{
	if (lMethodNum >= eMethodLast)
		return false;

	return methodList[lMethodNum].hasRetVal;
}

bool ADDIN_API CAddInNative::CallAsProc(const long lMethodNum, tVariant * paParams, const long lSizeArray)
{
	if (lMethodNum >= eMethodLast)
		return false;

	if (methodList[lMethodNum].hasRetVal == TRUE)
		return false;
	
	return (this->*(methodList[lMethodNum].pProc))(paParams);
}

bool ADDIN_API CAddInNative::CallAsFunc(const long lMethodNum, tVariant * pvarRetValue, tVariant * paParams, const long lSizeArray)
{
	if (lMethodNum >= eMethodLast)
		return false;

	if (methodList[lMethodNum].hasRetVal == FALSE)
		return false;

	return (this->*(methodList[lMethodNum].pFunc))(pvarRetValue, paParams);
}

void ADDIN_API CAddInNative::SetLocale(const WCHAR_T * loc)
{
	_wsetlocale(LC_ALL, loc);
}