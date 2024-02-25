#pragma once

#ifndef AFUTIL_PATH_H
#define AFUTIL_PATH_H

#include "afutil_base.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus
	typedef enum afutil_rcs
	{
		afutil_rcs_cancel,
		afutil_rcs_instead,
		afutil_rcs_save
	} afutil_rcs;
	typedef afutil_rcs(*afutil_rename_callback_multibyte)(const char* old_path, const char* new_path);
	typedef afutil_rcs(*afutil_rename_callback_wide)(const wchar_t* old_path, const wchar_t* new_path);

	AFUTIL_EXPORT int afutil_path_local_file_multibyte(afutil_handle handle, char* buffer, uint32_t bufelemsize);
	AFUTIL_EXPORT int afutil_path_local_file_wide(afutil_handle handle, wchar_t* buffer, uint32_t bufelemsize);
	AFUTIL_EXPORT int afutil_path_local_dir_multibyte(afutil_handle handle, char* buffer, uint32_t bufelemsize);
	AFUTIL_EXPORT int afutil_path_local_dir_wide(afutil_handle handle, wchar_t* buffer, uint32_t bufelemsize);
	AFUTIL_EXPORT int afutil_path_temporary_multibyte(char* buffer, uint32_t bufelemsize);
	AFUTIL_EXPORT int afutil_path_temporary_wide(wchar_t* buffer, uint32_t bufelemsize);
	AFUTIL_EXPORT afutil_bool afutil_path_rename_multibyte(const char* old_path, const char* new_path);
	AFUTIL_EXPORT afutil_bool afutil_path_rename_wide(const wchar_t* old_path, const wchar_t* new_path);
	AFUTIL_EXPORT afutil_bool afutil_path_rename_adv_multibyte(const char* old_path, const char* new_path, afutil_rename_callback_multibyte callback);
	AFUTIL_EXPORT afutil_bool afutil_path_rename_adv_wide(const wchar_t* old_path, const wchar_t* new_path, afutil_rename_callback_wide callback);
#ifdef __cplusplus
}
#endif // __cplusplus

#endif // !AFUTIL_PATH_H
