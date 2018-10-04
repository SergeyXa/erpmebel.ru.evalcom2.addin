#include "stdafx.h"
#include "Interpreter.h"

#include "bytecode.h"

using Bytecode::Instruction;

void CInterpreter::Load(std::string program)
{
	this->program = program;
	values.clear();
}

void CInterpreter::SetValue(double value)
{
	values.push_back(value);
}

void CInterpreter::SetValue(bool value)
{
	values.push_back(value ? 1.0f : 0.0f);
	values.push_back(value);
}

void CInterpreter::SetValue(int value)
{
	values.push_back(double(value));
}

bool CInterpreter::Execute(double& result)
{
	Instruction* code = (Instruction*)program.c_str();
	size_t codesz = program.size() / sizeof(Instruction);

	unsigned char* varcount = (unsigned char*)code++; --codesz;

	if (*varcount != values.size())
	{
#ifdef _FILE
		char msg[255];
		FILE* f = fopen("C:\\Junk\\Execute.txt", "wb+");
		fprintf(f, "*varcount %d != values.size() %d", *varcount, values.size());
		fclose(f);
#endif
		return false;
	}
		
	result = Bytecode::Execute(code, codesz, values.data());

	if (!isfinite(result))
		result = 0.0f;

	return true;
}
