#pragma once
#include <atlbase.h>

#include "Compiler.h"
#include "Interpreter.h"

class CAddInComCore :
	private CCompiler,
	private CInterpreter
{
public:
	HRESULT Compile(SAFEARRAY** paParams);
	HRESULT Load(VARIANT * pvarRetValue, SAFEARRAY** paParams);
	HRESULT SetValue(VARIANT * pvarRetValue, SAFEARRAY** paParams);
	HRESULT Execute(VARIANT * pvarRetValue, SAFEARRAY** paParams);

	HRESULT Version(VARIANT * pvarRetValue, SAFEARRAY** paParams);
};