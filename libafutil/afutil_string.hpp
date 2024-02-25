#pragma once

#ifndef AFUTIL_STRING_HPP
#define AFUTIL_STRING_HPP

#include "afutil_string.h"

#ifdef __cplusplus

#include <string>

namespace afutil
{
	namespace utf8_gbk
	{
		inline std::wstring convert_string(std::string string)
		{
			int size = afutil_convert_string_to_gbk(string.c_str(), nullptr, 0);
			wchar_t* temp_str = new wchar_t[size];
			afutil_convert_string_to_gbk(string.c_str(), temp_str, size);
			std::wstring ret_str = temp_str;
			delete[] temp_str;
			return ret_str;
		}

		inline std::string convert_string(std::wstring string)
		{
			int size = afutil_convert_string_to_utf8(string.c_str(), nullptr, 0);
			char* temp_str = new char[size];
			afutil_convert_string_to_utf8(string.c_str(), temp_str, size);
			std::string ret_str = temp_str;
			delete[] temp_str;
			return ret_str;
		}
	}
}

#endif // __cplusplus

#endif // !AFUTIL_STRING_HPP
