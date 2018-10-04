#pragma once

#include "AddInComCore.h"

#include "resource.h"
#include "generated\evalcom2_i.h"
#include "generated\addin_i.h"

using namespace ATL;

class ATL_NO_VTABLE CAddInCom :
	public CAddInComCore,
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CAddInCom, &CLSID_AddInCom>,
	public IDispatchImpl<IAddInCom, &IID_IAddInCom, &LIBID_evalcom2Lib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public IDispatchImpl<IInitDone, &__uuidof(IInitDone)>,
	public IDispatchImpl<ILanguageExtender, &__uuidof(ILanguageExtender)>,
	public IDispatchImpl<ILocale, &__uuidof(ILocale)>
{
public:
	CAddInCom()
	{
	}

	DECLARE_REGISTRY_RESOURCEID(IDR_ADDINCOM)


	BEGIN_COM_MAP(CAddInCom)
		COM_INTERFACE_ENTRY(IInitDone)
		COM_INTERFACE_ENTRY(ILanguageExtender)
		COM_INTERFACE_ENTRY(ILocale)
	END_COM_MAP()



	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
	}

public:


	// Inherited via IInitDone
private:
	IDispatch* m_connection;

public:
	virtual HRESULT STDMETHODCALLTYPE Init(IDispatch * pConnection) override;
	virtual HRESULT STDMETHODCALLTYPE Done(void) override;
	virtual HRESULT STDMETHODCALLTYPE GetInfo(SAFEARRAY ** pInfo) override;

	// Inherited via ILanguageExtender
	virtual HRESULT STDMETHODCALLTYPE RegisterExtensionAs(BSTR * bstrExtensionName) override;

	virtual HRESULT STDMETHODCALLTYPE GetNMethods(long * plMethods) override;
	virtual HRESULT STDMETHODCALLTYPE FindMethod(BSTR bstrMethodName, long * plMethodNum) override;
	virtual HRESULT STDMETHODCALLTYPE GetMethodName(long lMethodNum, long lMethodAlias, BSTR * pbstrMethodName) override;
	virtual HRESULT STDMETHODCALLTYPE GetNParams(long lMethodNum, long * plParams) override;
	virtual HRESULT STDMETHODCALLTYPE GetParamDefValue(long lMethodNum, long lParamNum, VARIANT * pvarParamDefValue) override;
	virtual HRESULT STDMETHODCALLTYPE HasRetVal(long lMethodNum, BOOL * pboolRetValue) override;

	virtual HRESULT STDMETHODCALLTYPE CallAsProc(long lMethodNum, SAFEARRAY ** paParams) override;
	virtual HRESULT STDMETHODCALLTYPE CallAsFunc(long lMethodNum, VARIANT * pvarRetValue, SAFEARRAY ** paParams) override;

	virtual HRESULT STDMETHODCALLTYPE GetNProps(long * plProps) override
	{
		*plProps = 0;
		return S_OK;
	}
	virtual HRESULT STDMETHODCALLTYPE FindProp(BSTR bstrPropName, long * plPropNum) override
	{
		return E_NOTIMPL;
	}
	virtual HRESULT STDMETHODCALLTYPE GetPropName(long lPropNum, long lPropAlias, BSTR * pbstrPropName) override
	{
		return E_NOTIMPL;
	}
	virtual HRESULT STDMETHODCALLTYPE GetPropVal(long lPropNum, VARIANT * pvarPropVal) override
	{
		return E_NOTIMPL;
	}
	virtual HRESULT STDMETHODCALLTYPE SetPropVal(long lPropNum, VARIANT * varPropVal) override
	{
		return E_NOTIMPL;
	}
	virtual HRESULT STDMETHODCALLTYPE IsPropReadable(long lPropNum, BOOL * pboolPropRead) override
	{
		return E_NOTIMPL;
	}
	virtual HRESULT STDMETHODCALLTYPE IsPropWritable(long lPropNum, BOOL * pboolPropWrite) override
	{
		return E_NOTIMPL;
	}

	// Inherited via ILocale
	virtual HRESULT STDMETHODCALLTYPE SetLocale(BSTR bstrLocale) override;

};

OBJECT_ENTRY_AUTO(__uuidof(AddInCom), CAddInCom)
