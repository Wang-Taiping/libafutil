#pragma once

#ifndef AFUTIL_IPC_H
#define AFUTIL_IPC_H

#include "afutil_base.h"

#define AFUTIL_PIPE_ACCESS_READ		(0x1)
#define AFUTIL_PIPE_ACCESS_WRITE	(0x2)
#define AFUTIL_PIPE_ACCESS_ALL		(0x3)

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus
	HANDLENAME(afutil_pipe);

	AFUTIL_EXPORT afutil_bool afutil_pipe_exists_multibyte(const char* pipe_name, uint32_t milliseconds);
	AFUTIL_EXPORT afutil_bool afutil_pipe_exists_wide(const wchar_t* pipe_name, uint32_t milliseconds);
	AFUTIL_EXPORT afutil_pipe afutil_pipe_create_multibyte(const char* pipe_name, int access_mode);
	AFUTIL_EXPORT afutil_pipe afutil_pipe_create_wide(const wchar_t* pipe_name, int access_mode);
	AFUTIL_EXPORT afutil_bool afutil_pipe_open(afutil_pipe pipe_handle);
	AFUTIL_EXPORT void afutil_pipe_close(afutil_pipe pipe_handle);
	AFUTIL_EXPORT afutil_pipe afutil_pipe_connect_multibyte(const char* pipe_name, int access_mode);
	AFUTIL_EXPORT afutil_pipe afutil_pipe_connect_wide(const wchar_t* pipe_name, int access_mode);
	AFUTIL_EXPORT void afutil_pipe_destroy(afutil_pipe pipe_handle);
	AFUTIL_EXPORT uint32_t afutil_pipe_read(afutil_pipe pipe_handle, void* buffer, uint32_t read_size);
	AFUTIL_EXPORT uint32_t afutil_pipe_write(afutil_pipe pipe_handle, const void* data, uint32_t write_size);
#ifdef __cplusplus
}
#endif // __cplusplus

#endif // !AFUTIL_IPC_H
