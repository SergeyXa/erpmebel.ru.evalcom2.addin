#pragma once

#include <string>

class CCompiler
{
public:
	bool CCompiler::Compile(wchar_t* expr, std::string& program, 
		IDispatch* namesdisp, IDispatch* contextdisp, IDispatch* errorsdisp);
};

