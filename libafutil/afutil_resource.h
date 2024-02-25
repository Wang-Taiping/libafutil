#pragma once

#ifndef AFUTIL_RESOURCE_H
#define AFUTIL_RESOURCE_H

#include "afutil_base.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus
	AFUTIL_EXPORT size_t afutil_resource_load_multibyte(void* hmodule, const char* type, int rscid, void** buffer);
	AFUTIL_EXPORT size_t afutil_resource_load_wide(void* hmodule, const wchar_t* type, int rscid, void** buffer);
#ifdef __cplusplus
}
#endif // __cplusplus

#endif // !AFUTIL_RESOURCE_H
