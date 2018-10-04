#include "stdafx.h"
#include "Compiler.h"
#include "ErrorDescription.h"

#include "generated\Parser.h"
#include "generated\Scanner.h"
#include "bytecode.h"

using namespace ATL;

bool CCompiler::Compile(wchar_t* expr, std::string& program, IDispatch* namesdisp, IDispatch* contextdisp, IDispatch* errorsdisp)
{
	Scanner s(expr, wcslen(expr));
	Parser p(&s);
	auto vc = p.gen.write((unsigned char)0u); // first int of bytecode is variable count
	p.Parse();
	if (p.errors->count == 0)
	{
		*vc = (unsigned char)p.context.size();

		program = std::string((char*)p.gen.bytecode(), p.gen.size() * sizeof(Instruction));

		// resolving names

		CComDispatchDriver names(namesdisp);
		CComDispatchDriver context(contextdisp);

		for (auto it = p.context.begin(); it != p.context.end(); ++it)
		{
			const std::wstring& name = it->first;
			CComVariant params[3];
			params[2] = name.c_str();
			params[1] = true;

			CComVariant propv;
			if (names.InvokeN(L"FindByDescription", params, 3, &propv) != S_OK)
			{
				p.errors->Error(0, 0, L"Фатальный сбой при поиске свойства");
				break;
			}

			CComDispatchDriver prop(V_DISPATCH(&propv));
			CComVariant propempty;
			prop.Invoke0(L"IsEmpty", &propempty);
			if (V_BOOL(&propempty) != FALSE)
			{
				wchar_t msg[78];
				wsprintf(msg, L"Не найдено свойство '%.50s'", it->first.c_str());
				p.errors->Error(it->second.line, it->second.col, msg);
				break;
			}

			context.Invoke1(L"Add", &propv);
		}
	}
	if (p.errors->count > 0)
	{
		CComDispatchDriver errors(errorsdisp);

		for (auto it = p.errors->errors.begin(); it != p.errors->errors.end(); ++it)
		{
			CComObject<CErrorDescription>* e;
			CComObject<CErrorDescription>::CreateInstance(&e);
			e->Init(*it);

			errors.Invoke1(L"Add", &CComVariant(e));
		}
	}

	return p.errors->count == 0;
}
