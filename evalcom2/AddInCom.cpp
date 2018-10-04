#include "stdafx.h"
#include "locale.h"
#include "AddInCom.h"

typedef HRESULT(CAddInCom::*MethodProc)(SAFEARRAY** paParams);
typedef HRESULT(CAddInCom::*MethodFunc)(VARIANT * pvarRetValue, SAFEARRAY** paParams);

enum MethodId {
	eMethodCompile,
	eMethodLoad,
	eMethodSetValue,
	eMethodExecute,
	eMethodLast
};

struct MethodInfo {
	MethodId id;
	wchar_t* name;
	wchar_t* nameRu;
	long nParams;
	BOOL hasRetVal;
	MethodProc pProc;
	MethodFunc pFunc;
};

MethodInfo methodList[eMethodLast] = {
	{ eMethodCompile,  L"Compile",  L"Скомпилировать",     5, FALSE, &CAddInCom::Compile, nullptr },
	{ eMethodLoad,     L"Load",     L"Загрузить",          1, TRUE,  nullptr, &CAddInCom::Load },
	{ eMethodSetValue, L"SetValue", L"УстановитьЗначение", 1, TRUE,  nullptr, &CAddInCom::SetValue },
	{ eMethodExecute,  L"Execute",  L"Выполнить",          0, TRUE,  nullptr, &CAddInCom::Execute }
};

//////////////////////////////////////////////////////////////////

#pragma warning(disable : 4996)

HRESULT STDMETHODCALLTYPE CAddInCom::Init(IDispatch * pConnection)
{
	if (pConnection == nullptr)
		return E_FAIL;

	m_connection = pConnection;
	m_connection->AddRef();

	return S_OK;
}

HRESULT STDMETHODCALLTYPE CAddInCom::Done(void)
{
	if (m_connection)
		m_connection->Release();
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CAddInCom::GetInfo(SAFEARRAY ** pInfo)
{
	// Component should put supported component technology version 
	// in VARIANT at index 0     
	long lInd = 0;
	CComVariant varVersion;
	V_VT(&varVersion) = VT_I4;
	// This component supports 2.0 version
	V_I4(&varVersion) = 2000;
	SafeArrayPutElement(*pInfo, &lInd, &varVersion);

	return S_OK;
}

HRESULT STDMETHODCALLTYPE CAddInCom::RegisterExtensionAs(BSTR * bstrExtensionName)
{
	wchar_t* csExtenderName = L"Evaluator";
	*bstrExtensionName = ::SysAllocString(csExtenderName);

	return S_OK;
}

HRESULT STDMETHODCALLTYPE CAddInCom::GetNMethods(long * plMethods)
{
	*plMethods = eMethodLast;
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CAddInCom::FindMethod(BSTR bstrMethodName, long * plMethodNum)
{
	*plMethodNum = -1;
	for (int i = 0; i < eMethodLast; ++i)
		if (wcsicmp(bstrMethodName, methodList[i].name) == 0
			|| wcsicmp(bstrMethodName, methodList[i].nameRu) == 0)
		{
			*plMethodNum = i;
			break;
		}
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CAddInCom::GetMethodName(long lMethodNum, long lMethodAlias, BSTR * pbstrMethodName)
{
	if (lMethodNum >= eMethodLast)
		return E_INVALIDARG;

	*pbstrMethodName = ::SysAllocString(lMethodAlias == 0
		? methodList[lMethodNum].name
		: methodList[lMethodNum].nameRu);
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CAddInCom::GetNParams(long lMethodNum, long * plParams)
{
	if (lMethodNum >= eMethodLast)
		return E_INVALIDARG;

	*plParams = methodList[lMethodNum].nParams;
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CAddInCom::GetParamDefValue(long lMethodNum, long lParamNum, VARIANT * pvarParamDefValue)
{
	V_VT(pvarParamDefValue) = VT_EMPTY;
	return S_FALSE;
}

HRESULT STDMETHODCALLTYPE CAddInCom::HasRetVal(long lMethodNum, BOOL * pboolRetValue)
{
	if (lMethodNum >= eMethodLast)
		return E_INVALIDARG;

	*pboolRetValue = methodList[lMethodNum].hasRetVal;
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CAddInCom::CallAsProc(long lMethodNum, SAFEARRAY ** paParams)
{
	if (lMethodNum >= eMethodLast)
		return E_INVALIDARG;

	if (methodList[lMethodNum].hasRetVal == TRUE)
		return E_INVALIDARG;

	return (this->*(methodList[lMethodNum].pProc))(paParams);
}

HRESULT STDMETHODCALLTYPE CAddInCom::CallAsFunc(long lMethodNum, VARIANT * pvarRetValue, SAFEARRAY ** paParams)
{
	if (lMethodNum >= eMethodLast)
		return E_INVALIDARG;

	if (methodList[lMethodNum].hasRetVal == FALSE)
		return E_INVALIDARG;

	return (this->*(methodList[lMethodNum].pFunc))(pvarRetValue, paParams);
}

HRESULT STDMETHODCALLTYPE CAddInCom::SetLocale(BSTR bstrLocale)
{
	_wsetlocale(LC_ALL, bstrLocale);
	return S_OK;
}
