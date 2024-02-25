#include "afutil_string.h"
#include <Windows.h>

int afutil_convert_string_to_gbk(const char* string, wchar_t* buffer, int bufelemsize)
{
    return MultiByteToWideChar(936, 0, string, -1, buffer, bufelemsize);
}

int afutil_convert_string_to_utf8(const wchar_t* string, char* buffer, int bufelemsize)
{
    return WideCharToMultiByte(65001, 0, string, -1, buffer, bufelemsize, nullptr, 0);
}
