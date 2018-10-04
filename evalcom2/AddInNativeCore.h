#pragma once

#include "1c\types.h"
#include "1c\AddInDefBase.h"
#include "1c\IMemoryManager.h"

#include "Interpreter.h"

class CAddInNativeCore :
	private CInterpreter
{
protected:
	IAddInDefBase* m_defBase;
	IMemoryManager* m_memMan;

	CAddInNativeCore();
public:
	bool Load(tVariant * pvarRetValue, tVariant * paParams);
	bool SetValue(tVariant * pvarRetValue, tVariant * paParams);
	bool Execute(tVariant * pvarRetValue, tVariant * paParams);

	bool Version(tVariant * pvarRetValue, tVariant * paParams);
};

