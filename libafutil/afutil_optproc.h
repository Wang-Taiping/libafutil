#pragma once

#ifndef AFUTIL_OPTPROC_H
#define AFUTIL_OPTPROC_H

#include "afutil_base.h"

#define AFUTIL_OVERFLOW		int(-1)
#define AFUTIL_IGNORE		int(-2)
#define AFUTIL_NOT_EXIST	int(-3)
#define AFUTIL_NORMAL		int(-4)

#define AFUTIL_ARGNUM(x)	int(x)
#define AFUTIL_NOARG		AFUTIL_ARGNUM(0)
#define AFUTIL_REQUIRE		AFUTIL_ARGNUM(1)
#define AFUTIL_OPTIONAL		AFUTIL_ARGNUM(-1)

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus
	HANDLENAME(afutil_optman);
	HANDLENAME(afutil_woptman);
	typedef struct afutil_optrule
	{
		const char* name;
		uint32_t arg_number;
		int ret_number;
	} afutil_optrule;
	typedef struct afutil_woptrule
	{
		const wchar_t* name;
		uint32_t arg_number;
		int ret_number;
	} afutil_woptrule;

	AFUTIL_EXPORT afutil_optman afutil_opt_create();
	AFUTIL_EXPORT void afutil_opt_destroy(afutil_optman opt_handle);
	AFUTIL_EXPORT afutil_bool afutil_opt_bind(afutil_optman opt_handle, int argc, char* argv[]);
	AFUTIL_EXPORT int afutil_opt_proc(afutil_optman opt_handle, afutil_optrule* rules);
	AFUTIL_EXPORT const char* afutil_opt_result(afutil_optman opt_handle, int32_t index);

	AFUTIL_EXPORT afutil_woptman afutil_wopt_create();
	AFUTIL_EXPORT void afutil_wopt_destroy(afutil_woptman opt_handle);
	AFUTIL_EXPORT afutil_bool afutil_wopt_bind(afutil_woptman opt_handle, int argc, wchar_t* argv[]);
	AFUTIL_EXPORT int afutil_wopt_proc(afutil_woptman opt_handle, afutil_woptrule* rules);
	AFUTIL_EXPORT const wchar_t* afutil_wopt_result(afutil_woptman opt_handle, int32_t index);
#ifdef __cplusplus
}
#endif // __cplusplus

#endif // !AFUTIL_OPTPROC_H
