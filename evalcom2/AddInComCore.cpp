#include "stdafx.h"
#include "AddInComCore.h"
#include "Base64.h"
#include "generated\Scanner.h"
#include "generated\Parser.h"
#include "ErrorDescription.h"

using namespace ATL;

CComVariant GetNParam(SAFEARRAY * pArray, long lIndex)
{
	_ASSERT(pArray);
	_ASSERT(pArray->fFeatures | FADF_VARIANT);

	CComVariant vt;
	HRESULT hRes = ::SafeArrayGetElement(pArray, &lIndex, &vt);
	_ASSERT(hRes == S_OK);

	return vt;
}

void SetNParam(SAFEARRAY * pArray, long lIndex, CComVariant& value)
{
	_ASSERT(pArray);
	_ASSERT(pArray->fFeatures | FADF_VARIANT);

	HRESULT hRes = ::SafeArrayPutElement(pArray, &lIndex, &value);

	_ASSERT(hRes == S_OK);
}

HRESULT CAddInComCore::Compile(SAFEARRAY ** paParams)
{
	CComVariant exprv		= GetNParam(*paParams, 0);
	CComVariant contextv	= GetNParam(*paParams, 2);
	CComVariant namesv		= GetNParam(*paParams, 3);
	CComVariant errorsv		= GetNParam(*paParams, 4);

	if (exprv.vt != VT_BSTR) return S_FALSE;
	if (contextv.vt != VT_DISPATCH) return S_FALSE;
	if (namesv.vt != VT_DISPATCH) return S_FALSE;
	if (errorsv.vt != VT_DISPATCH) return S_FALSE;

	std::string program;
	
	CCompiler::Compile(exprv.bstrVal, program,
		V_DISPATCH(&namesv), V_DISPATCH(&contextv), V_DISPATCH(&errorsv));

	CComVariant codev = CComVariant(Base64::Encode(program.c_str(), program.size()).c_str());
	SetNParam(*paParams, 1, codev);

	return S_OK;
}

HRESULT CAddInComCore::Load(VARIANT * pvarRetValue, SAFEARRAY ** paParams)
{
	CComVariant programv = GetNParam(*paParams, 0);
	if (programv.vt != VT_BSTR) return S_FALSE;

	CInterpreter::Load(Base64::Decode(programv.bstrVal, wcslen(programv.bstrVal)));

	pvarRetValue->vt = VT_BOOL;
	pvarRetValue->boolVal = VARIANT_TRUE;

	return S_OK;
}

HRESULT CAddInComCore::SetValue(VARIANT * pvarRetValue, SAFEARRAY ** paParams)
{
	CComVariant valuev = GetNParam(*paParams, 0);

	pvarRetValue->vt = VT_BOOL;
	pvarRetValue->boolVal = VARIANT_TRUE;

	switch (valuev.vt)
	{
	case VT_R8:
		CInterpreter::SetValue(V_R8(&valuev));
		break;
	case VT_I4:
		CInterpreter::SetValue(V_I4(&valuev));
		break;
	case VT_BOOL:
		CInterpreter::SetValue(V_BOOL(&valuev) == VARIANT_TRUE);
		break;
	default:
		pvarRetValue->boolVal = VARIANT_FALSE;
	}

	return S_OK;
}

HRESULT CAddInComCore::Execute(VARIANT * pvarRetValue, SAFEARRAY ** paParams)
{
	double result;
	if (CInterpreter::Execute(result))
	{
		pvarRetValue->vt = VT_R8;
		pvarRetValue->dblVal = result;
	}
	else
		pvarRetValue->vt = VT_EMPTY;

	return S_OK;
}

HRESULT CAddInComCore::Version(VARIANT * pvarRetValue, SAFEARRAY ** paParams)
{
#ifdef _WIN64
	BSTR version = L"COM x64";
#else
	BSTR version = L"COM x86";
#endif

	pvarRetValue->bstrVal = ::SysAllocString(version);
	pvarRetValue->vt = VT_BSTR;
	
	return S_OK;
}