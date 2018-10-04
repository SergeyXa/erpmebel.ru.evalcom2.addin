#pragma once

#include <math.h>
#include <limits.h>
#include <stdio.h>
#include <Windows.h>
#include <vector>

namespace Bytecode {

	enum Instruction : unsigned char {
		PUSH, // <double>
		PUSHVAR, // <char>
		POW,
		MUL,
		DIV,
		MOD,
		ADD,
		SUB,
		JUMP, // <short>
		JZ, // <short>
		EQ,
		NE,
		GT,
		LT,
		LE,
		GE,
		NOT,
		AND,
		OR,
		MIN,
		MAX,
		ROUND,
		CEIL,
		FLOOR,
		ROOT,
		NMPOS,
		IDIV,
		PUSHNULL,
		SUBMAX,
		SUBMIN
	};

	__forceinline double Execute(
		Instruction* code, size_t codesize,
		double* vars)
	{
		Instruction* eip = (Instruction*)code; // instruction pointer

		double stack[256];
		double* esp = stack;

		double d;
		//int stackmax;

		while (eip < (Instruction*)(code + codesize))
		{
			switch (*eip)
			{
			case PUSH:
				++eip;
				//wprintf(L"PUSH %f", *(double*)eip);
				*esp++ = *(double*)eip;
				//stackmax = max(stackmax, esp - stack);
				eip += sizeof(double) / sizeof(Instruction);
				break;
			case PUSHVAR:
				++eip;
				//wprintf(L"PUSHVAR %d", *eip);
				*esp++ = vars[*eip];
				//stackmax = max(stackmax, esp - stack);
				eip += 1;
				break;
			case POW:
				//wprintf(L"POW");
				esp -= 2;
				*esp++ = pow(*(esp), *(esp + 1));
				++eip;
				break;
			case MUL:
				//wprintf(L"MUL");
				esp -= 2;
				*esp++ = *(esp) * *(esp + 1);
				++eip;
				break;
			case DIV:
				//wprintf(L"DIV");
				esp -= 2;
				*esp++ = *(esp) / *(esp + 1);
				++eip;
				break;
			case IDIV:
				//wprintf(L"DIV");
				esp -= 2;
				*esp++ = floor(*(esp) / *(esp + 1));
				++eip;
				break;
			case MOD:
				//wprintf(L"MOD");
				esp -= 2;
				*esp++ = fmod(*(esp), *(esp + 1));
				++eip;
				break;
			case ADD:
				//wprintf(L"ADD");
				esp -= 2;
				*esp++ = *(esp)+*(esp + 1);
				++eip;
				break;
			case SUB:
				//wprintf(L"SUB");
				esp -= 2;
				*esp++ = *(esp)-*(esp + 1);
				++eip;
				break;
			case JUMP:
				++eip;
				//wprintf(L"JUMP %d", *eip);
				eip += *(unsigned short*)eip;
				eip += sizeof(unsigned short) / sizeof(Instruction);
				break;
			case JZ:
				++eip;
				//wprintf(L"JZ %d", *eip);
				--esp;
				if (*esp == 0.0f || (isnan(*esp)))
					eip += *(unsigned short*)eip;
				eip += sizeof(unsigned short) / sizeof(Instruction);
				break;
			case EQ:
				//wprintf(L"EQ");
				esp -= 2;
				*esp++ = (*(esp) == *(esp + 1) || (isnan(*esp) && isnan(*(esp + 1)))) ? 1.0f : 0.0f;
				++eip;
				break;
			case NE:
				//wprintf(L"NE");
				esp -= 2;
				*esp++ = (*(esp) == *(esp + 1) || (isnan(*esp) && isnan(*(esp + 1)))) ? 0.0f : 1.0f;
				++eip;
				break;
			case GT:
				//wprintf(L"GT");
				esp -= 2;
				*esp++ = *(esp) > *(esp + 1) ? 1.0f : 0.0f;
				++eip;
				break;
			case LT:
				//wprintf(L"LT");
				esp -= 2;
				*esp++ = *(esp) < *(esp + 1) ? 1.0f : 0.0f;
				++eip;
				break;
			case LE:
				//wprintf(L"LE");
				esp -= 2;
				*esp++ = *(esp) <= *(esp + 1) ? 1.0f : 0.0f;
				++eip;
				break;
			case GE:
				//wprintf(L"LE");
				esp -= 2;
				*esp++ = *(esp) >= *(esp + 1) ? 1.0f : 0.0f;
				++eip;
				break;
			case NOT:
				//wprintf(L"NOT");
				*(esp - 1) = *(esp - 1) == 0.0f ? 1.0f : 0.0f;
				++eip;
				break;
			case AND:
				//wprintf(L"AND");
				esp -= 2;
				*esp++ = (*(esp) == 0.0f || *(esp + 1) == 0.0f) ? 0.0f : 1.0f;
				++eip;
				break;
			case OR:
				//wprintf(L"OR");
				esp -= 2;
				*esp++ = (*(esp) == 0.0f && *(esp + 1) == 0.0f) ? 0.0f : 1.0f;
				++eip;
				break;
			case MIN:
				//wprintf(L"MIN");
				esp -= 2;
				*esp++ = *(esp) < *(esp + 1) ? *(esp) : *(esp + 1);
				++eip;
				break;
			case MAX:
				//wprintf(L"MIN");
				esp -= 2;
				*esp++ = *(esp) > *(esp + 1) ? *(esp) : *(esp + 1);
				++eip;
				break;
			case ROUND:
				//wprintf(L"ROUND");
				esp -= 2;
				d = pow(10., *(esp + 1));
				*esp++ = round(*(esp)* d) / d;
				++eip;
				break;
			case CEIL:
				//wprintf(L"CEIL");
				esp -= 2;
				d = pow(10., *(esp + 1));
				*esp++ = ceil(*(esp)* d) / d;
				++eip;
				break;
			case FLOOR:
				//wprintf(L"FLOOR");
				esp -= 2;
				d = pow(10., *(esp + 1));
				*esp++ = floor(*(esp)* d) / d;
				++eip;
				break;
			case ROOT:
				//wprintf(L"ROOT");
				esp -= 2;
				if (*(esp + 1) == 2.0f)
					*esp++ = sqrt(*esp);
				else
					*esp++ = pow(*esp, 1. / *(esp + 1));
				++eip;
				break;
			case NMPOS:
				//wprintf(L"NMPOS");
				esp -= 2;
				*esp++ = fmod(floor(*(esp) / pow(10., *(esp + 1) - 1.)), 10.);
				++eip;
				break;
			case PUSHNULL:
				//wprintf(L"NULL", *(double*)eip);
				*esp++ = nan(0);
				++eip;
				break;
			case SUBMAX:
				{
					size_t count = size_t(*--esp);
					double* args = new double[count];
					if (args)
						for (int i = 0; i < count; i++)
							args[i] = *--esp;
					int which = int(*--esp);
					std::qsort(
						args,
						count, sizeof(double),
						[](const void* a, const void* b)
					{
						return
							*(double*)a > *(double*)b
							? -1
							: *(double*)a == *(double*)b ? 0 : 1;
					});
					*esp++ = which <= count ? args[max(0, which - 1)] : nan(0);
					++eip;
					delete[](args);
				} break;
			case SUBMIN:
				{
					size_t count = size_t(*--esp);
					double* args = new double[count];
					if (args)
						for (int i = 0; i < count; i++)
							args[i] = *--esp;
					int which = int(*--esp);
					std::qsort(
						args,
						count, sizeof(double),
						[](const void* a, const void* b)
					{
						return
							*(double*)a < *(double*)b
							? -1
							: *(double*)a == *(double*)b ? 0 : 1;
					});
					*esp++ = which <= count ? args[max(0, which - 1)] : nan(0);
					++eip;
					delete[](args);
				} break;
			}
		}
		//char msg[255];
		//sprintf(msg, "max stack is: %d", stackmax);
		//MessageBoxA(NULL, msg, NULL, NULL);

		return stack[0];
	}

	template <class T, size_t CAP = 256>
	class Bytegen
	{
#ifdef _FILE
		FILE* f;
#endif
		size_t _cap;
		T* _buf;
		T* _bufp; // first free position in buffer
		T* _bufe; // last position in buffer
	public:
		Bytegen()
		{
#ifdef _FILE
			f = fopen("C:\\Junk\\bytegen.txt", "wb");
#endif
			_cap = CAP;
			_buf = (T*)malloc(sizeof(T) * _cap);
			_bufe = _buf + _cap;
			_bufp = _buf;
		}
		~Bytegen()
		{
#ifdef _FILE
			fclose(f);
#endif
			free(_buf);
		}
	protected:
		void ensurefits(size_t n)
		{
			if (_bufp + n >= _bufe)
			{
				size_t p = _bufp - _buf;
				_cap *= 2;
				_buf = (T*)realloc(_buf, sizeof(T) * _cap);
				_bufe = _buf + _cap;
				_bufp = _buf + p;
			}
		}
	public:
		T* bytecode() const
		{
			return _buf;
		}
		size_t size() const
		{
			return _bufp - _buf;
		}

		template<class TD>
		class pointer
		{
			Bytegen* gen;
			size_t ofs;
		public:
			pointer(Bytegen* gen, void* pointer)
			{
				this->gen = gen;
				this->ofs = (char*)pointer - (char*)gen->_buf;
			}
			TD& operator* ()
			{
				return *(TD*)((char*)gen->_buf + ofs);
			}
		};

		template<class TD> pointer<TD> write(const TD& data)
		{
			ensurefits(sizeof(TD) / sizeof(T));
			TD* r = (TD*)_bufp;
			*r = data;
			_bufp += sizeof(TD) / sizeof(T);
			return pointer<TD>(this, (void*)r);
		}
#ifdef _FILE
		template<> pointer<Instruction> write(const Instruction& data)
		{
			switch (data)
			{
			case PUSH: fprintf(f, "PUSH\n"); break;
			case PUSHVAR: fprintf(f, "PUSHVAR\n"); break;
			case POW: fprintf(f, "POW\n"); break;
			case MUL: fprintf(f, "MUL\n"); break;
			case DIV: fprintf(f, "DIV\n"); break;
			case MOD: fprintf(f, "MOD\n"); break;
			case ADD: fprintf(f, "ADD\n"); break;
			case SUB: fprintf(f, "SUB\n"); break;
			case JUMP: fprintf(f, "JUMP\n"); break;
			case JZ: fprintf(f, "JZ\n"); break;
			case EQ: fprintf(f, "EQ\n"); break;
			case NE: fprintf(f, "NE\n"); break;
			case GT: fprintf(f, "GT\n"); break;
			case LT: fprintf(f, "LT\n"); break;
			case LE: fprintf(f, "LE\n"); break;
			case GE: fprintf(f, "GE\n"); break;
			case NOT: fprintf(f, "NOT\n"); break;
			case AND: fprintf(f, "AND\n"); break;
			case OR: fprintf(f, "OR\n"); break;
			case MIN: fprintf(f, "MIN\n"); break;
			case MAX: fprintf(f, "MAX\n"); break;
			case ROUND: fprintf(f, "ROUND\n"); break;
			case CEIL: fprintf(f, "CEIL\n"); break;
			case FLOOR: fprintf(f, "FLOOR\n"); break;
			case ROOT: fprintf(f, "ROOT\n"); break;
			};
			ensurefits(1);
			Instruction* r = (Instruction*)_bufp;
			*r = data;
			_bufp += 1;
			return pointer<Instruction>(this, (void*)r);
		}
#endif
	};

}