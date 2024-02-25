#pragma once

#ifndef AFUTIL_OPTPROC_HPP
#define AFUTIL_OPTPROC_HPP

#include "afutil_optproc.h"

#ifdef __cplusplus

namespace afutil
{
	typedef afutil_optrule optrule;
	typedef afutil_woptrule woptrule;

	class optman
	{
	public:
		optman()
		{
			hoptman = afutil_opt_create();
		}
		~optman()
		{
			afutil_opt_destroy(hoptman);
		}
		bool bind(int argc, char* argv[])
		{
			return afutil_opt_bind(hoptman, argc, argv);
		}
		int proc(afutil_optrule* rules)
		{
			return afutil_opt_proc(hoptman, rules);
		}
		const char* result(int index)
		{
			return afutil_opt_result(hoptman, index);
		}
	private:
		afutil_optman hoptman;
	};

	class woptman
	{
	public:
		woptman()
		{
			hoptman = afutil_wopt_create();
		}
		~woptman()
		{
			afutil_wopt_destroy(hoptman);
		}
		bool bind(int argc, wchar_t* argv[])
		{
			return afutil_wopt_bind(hoptman, argc, argv);
		}
		int proc(afutil_woptrule* rules)
		{
			return afutil_wopt_proc(hoptman, rules);
		}
		const wchar_t* result(int index)
		{
			return afutil_wopt_result(hoptman, index);
		}
	private:
		afutil_woptman hoptman;
	};
}

#endif // __cplusplus

#endif // !AFUTIL_OPTPROC_HPP
