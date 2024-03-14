#pragma once

#ifndef AFUTIL_BASE_H
#define AFUTIL_BASE_H

#include <stdint.h>

#ifdef AFUTIL_BUILD
#define AFUTIL_EXPORT	__declspec(dllexport)
#else
#define AFUTIL_EXPORT	__declspec(dllimport)
#endif // AFUTIL_BUILD

#define HANDLENAME(name)	struct __##name { int unused; }; typedef __##name *name;

typedef void* afutil_handle;
typedef char afutil_bool;

typedef void(*afutil_callback_t)(void* user_data);

#endif // !AFUTIL_BASE_H
