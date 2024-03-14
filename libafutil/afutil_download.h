#pragma once

#ifndef AFUTIL_DOWNLOAD_H
#define AFUTIL_DOWNLOAD_H

#include "afutil_base.h"
#include "afutil_timer.h"
#include <stdio.h>

#define AFUTIL_DOWNLOAD_URL_DEFAULT_LENGTH	(128)

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus
	typedef enum afutil_download_operation
	{
		afutil_download_operation_continue,
		afutil_download_operation_pause,
		afutil_download_operation_stop
	} afutil_download_operation;

	typedef struct afutil_download_info {
		uint64_t total;
		uint64_t downloaded;
		uint64_t speed;
		float percentage;
		struct {
			uint32_t day;
			uint8_t hour;
			uint8_t min;
			uint8_t sec;
		} eta;
	} afutil_download_info;

	typedef struct afutil_download_native_attribute
	{
		bool exit;
		bool fail;
		bool retry;
		bool alloc;
	} afutil_download_native_attribute;

	typedef struct afutil_download_native_handle
	{
		void* curl;
		uint64_t info_last_downloaded;
		uint64_t cached;
		uint64_t storaged;
		uint32_t lsbcount;
		afutil_timer timer;
		afutil_download_native_attribute attribute;
	} afutil_download_native_handle;

	typedef afutil_download_operation(*afutil_download_callback)(const afutil_download_info* info, void* user_data);

	typedef struct afutil_download_metadata {
		char url[AFUTIL_DOWNLOAD_URL_DEFAULT_LENGTH];
		char* extended_url;
		FILE* file;
		char* buffer;
		uint64_t buflen;
		afutil_download_callback callback;
		void* user_data;
		afutil_download_info info;
		afutil_download_native_handle native; // do not change!!!
	} afutil_download_metadata;

	AFUTIL_EXPORT afutil_bool afutil_download_init();
	AFUTIL_EXPORT afutil_bool afutil_download_uninit();

	AFUTIL_EXPORT afutil_bool afutil_download(afutil_download_metadata* metadata);
	AFUTIL_EXPORT afutil_bool afutil_download_file(const char* url, const char* path, afutil_download_callback callback, void* user_data);
	AFUTIL_EXPORT afutil_bool afutil_download_api(const char* url, void* buffer, uint64_t buflen, uint64_t* downloaded);
#ifdef __cplusplus
}
#endif // __cplusplus

#endif // !AFUTIL_DOWNLOAD_KERNEL_H
