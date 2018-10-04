#include "Base64.h"
#include <string>
#include <vector>

namespace Base64
{

	std::wstring Encode(const char data[], size_t datalen)
	{
		std::wstring out;

		const unsigned char* dataend = (unsigned char*)data + datalen;
		const unsigned char* c = (unsigned char*)data;
		int val = 0, valb = -6;
		while (c < dataend)
		{
			val = (val << 8) + *c;
			valb += 8;
			while (valb >= 0)
			{
				out.push_back(L"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[(val >> valb) & 0x3F]);
				valb -= 6;
			}
			++c;
		}
		if (valb > -6) out.push_back(L"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[((val << 8) >> (valb + 8)) & 0x3F]);
		while (out.size() % 4) out.push_back(L'=');
		return out;
	}

	std::string Decode(const wchar_t data[], size_t datalen)
	{
		std::string out;

		std::vector<int> T(256, -1);
		for (int i = 0; i < 64; i++) T[L"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[i]] = i;

		const wchar_t* dataend = (wchar_t*)data + datalen;
		const wchar_t* c = (wchar_t*)data;
		int val = 0, valb = -8;
		while (c < dataend)
		{
			if (T[(wchar_t)*c] == -1) break;
			val = (val << 6) + T[*c];
			valb += 6;
			if (valb >= 0)
			{
				out.push_back(char((val >> valb) & 0xFF));
				valb -= 8;
			}
			++c;
		}
		return out;
	}

	std::string Decode(const char data[], size_t datalen)
	{
		std::string out;

		std::vector<int> T(256, -1);
		for (int i = 0; i < 64; i++) T[L"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[i]] = i;

		const char* dataend = (char*)data + datalen;
		const char* c = (char*)data;
		int val = 0, valb = -8;
		while (c < dataend)
		{
			if (T[(char)*c] == -1) break;
			val = (val << 6) + T[*c];
			valb += 6;
			if (valb >= 0)
			{
				out.push_back(char((val >> valb) & 0xFF));
				valb -= 8;
			}
			++c;
		}
		return out;
	}


}