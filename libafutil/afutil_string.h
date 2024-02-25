#pragma once

#ifndef AFUTIL_STRING_H
#define AFUTIL_STRING_H

#include "afutil_base.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

	AFUTIL_EXPORT int afutil_convert_string_to_gbk(const char* string, wchar_t* buffer, int bufelemsize);
	AFUTIL_EXPORT int afutil_convert_string_to_utf8(const wchar_t* string, char* buffer, int bufelemsize);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // !AFUTIL_STRING_H
