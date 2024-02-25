#pragma once

#ifndef AFUTIL_DOWNLOAD_H
#define AFUTIL_DOWNLOAD_H

#include "afutil_base.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus
	typedef afutil_bool(*afutil_download_callback_t)(uint64_t total_bytes, uint64_t download_bytes, uint64_t speed_bytes, void* user_data);

	AFUTIL_EXPORT afutil_bool afutil_download_file_multibyte(const char* url, const char* path, afutil_download_callback_t callback = nullptr, void* user_data = nullptr);
	AFUTIL_EXPORT afutil_bool afutil_download_file_wide(const wchar_t* url, const wchar_t* path, afutil_download_callback_t callback = nullptr, void* user_data = nullptr);
	AFUTIL_EXPORT afutil_bool afutil_download_api_multibyte(const char* url, void* buffer, size_t bufsize, size_t* download_size = nullptr);
	AFUTIL_EXPORT afutil_bool afutil_download_api_wide(const wchar_t* url, void* buffer, size_t bufsize, size_t* download_size = nullptr);
	AFUTIL_EXPORT size_t afutil_url_get_filename_multibyte(const char* url, char* buffer, size_t bufsize);
	AFUTIL_EXPORT size_t afutil_url_get_filename_wide(const wchar_t* url, wchar_t* buffer, size_t bufsize);
#ifdef __cplusplus
}
#endif // __cplusplus

#endif // !AFUTIL_DOWNLOAD_KERNEL_H
