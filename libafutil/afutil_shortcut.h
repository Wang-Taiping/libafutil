#pragma once

#ifndef AFUTIL_SHORTCUT_H
#define AFUTIL_SHORTCUT_H

#include "afutil_base.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus
	afutil_bool afutil_shortcut_create_multibyte(const char* source_file, const char* shortcut, const char* description, const char* working_dir, const char* icon_path, int icon_id = 0);
	afutil_bool afutil_shortcut_create_wide(const wchar_t* source_file, const wchar_t* shortcut, const wchar_t* description, const wchar_t* working_dir, const wchar_t* icon_path, int icon_id = 0);
#ifdef __cplusplus
}
#endif // __cplusplus

#endif // !AFUTIL_SHORTCUT_H
