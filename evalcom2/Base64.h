#pragma once

#include <string>

namespace Base64 {

	std::wstring Encode(const char data[], size_t datalen);
	std::string Decode(const wchar_t data[], size_t datalen);
	std::string Decode(const char data[], size_t datalen);

}
