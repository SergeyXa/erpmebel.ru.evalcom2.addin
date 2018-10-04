#pragma once

#include "resource.h"       // main symbols
#include "generated\evalcom2_i.h"
#include "generated\Parser.h"

using namespace ATL;

class ATL_NO_VTABLE CErrorDescription :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CErrorDescription, &CLSID_ErrorDescription>,
	public IDispatchImpl<IErrorDescription, &IID_IErrorDescription, &LIBID_evalcom2Lib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
private:
	wchar_t* msg;
	int line;
	int col;

public:
	void Init(Errors::Item& e)
	{
		line = e.line;
		col = e.col;
		msg = e.msg;
		e.msg = nullptr;
	}
	~CErrorDescription()
	{
		coco_string_delete(msg);
	}

	STDMETHOD(get_Line)(LONG* pVal)
	{
		*pVal = line;
		return S_OK;
	}
	STDMETHOD(get_Column)(LONG* pVal)
	{
		*pVal = col;
		return S_OK;
	}
	STDMETHOD(get_Description)(BSTR* pVal)
	{
		*pVal = ::SysAllocString(msg);
		return S_OK;
	}

public:
DECLARE_REGISTRY_RESOURCEID(IDR_ERRORDESCRIPTION)
BEGIN_COM_MAP(CErrorDescription)
	COM_INTERFACE_ENTRY(IErrorDescription)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()
	DECLARE_PROTECT_FINAL_CONSTRUCT()
};