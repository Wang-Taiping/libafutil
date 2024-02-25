#pragma once

#ifndef AFUTIL_OPTPROC_H
#define AFUTIL_OPTPROC_H

#include "afutil_base.h"

#define AFUTIL_OPT_OVERFLOW		int(-1)
#define AFUTIL_OPT_IGNORE		int(-2)
#define AFUTIL_OPT_NEXIST		int(-3)
#define AFUTIL_OPT_NORMAL		int(-4)

#define AFUTIL_OPT_ARGNUM(x)	int(x)
#define AFUTIL_OPT_NO_ARG		AFUTIL_OPT_ARGNUM(0)
#define AFUTIL_OPT_REQ_ARG		AFUTIL_OPT_ARGNUM(1)
#define AFUTIL_OPT_OPT_ARG		AFUTIL_OPT_ARGNUM(-1)

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus
	TYPENAME(afutil_optman);
	TYPENAME(afutil_woptman);
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

	afutil_optman afutil_opt_create();
	void afutil_opt_destroy(afutil_optman opt_handle);
	afutil_bool afutil_opt_bind(afutil_optman opt_handle, int argc, char* argv[]);
	int afutil_opt_proc(afutil_optman opt_handle, afutil_optrule* rules);
	const char* afutil_opt_result(afutil_optman opt_handle, uint32_t index);

	afutil_woptman afutil_wopt_create();
	void afutil_wopt_destroy(afutil_woptman opt_handle);
	afutil_bool afutil_wopt_bind(afutil_woptman opt_handle, int argc, wchar_t* argv[]);
	int afutil_wopt_proc(afutil_woptman opt_handle, afutil_woptrule* rules);
	const wchar_t* afutil_wopt_result(afutil_woptman opt_handle, uint32_t index);
#ifdef __cplusplus
}
#endif // __cplusplus

#endif // !AFUTIL_OPTPROC_H
