#pragma once

#include "generated\Scanner.h"

#include <string>
#include <map>

struct ContextItem
{
	char index;
	int line; // first occurence
	int col;
};

class Context :
	public std::map<std::wstring, ContextItem>
{
public:
	char varindex(Token* t, bool hasbrackets = false)
	{
		std::wstring name_;
		if (hasbrackets)
		{
			name_ = std::wstring(t->val + 1);
			name_.erase(name_.end() - 1);
		}
		else
			name_ = std::wstring(t->val);
		
		auto it = find(name_);
		if (it == end())
		{
			char index = char(size());
			(*this)[name_] = {index, t->line, t->col };
			return index;
		}
		else
			return it->second.index;
	}
};
