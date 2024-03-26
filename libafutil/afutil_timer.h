#pragma once

#ifndef AFUTIL_TIMER_H
#define AFUTIL_TIMER_H

#include "afutil_base.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus
	HANDLENAME(afutil_timer_handle);

	typedef struct afutil_timer
	{
		uint32_t loops;
		uint32_t wait_sec;
		uint32_t expired_sec;
		bool active_signal;
		afutil_callback_t callback;
		void* user_data;
	} afutil_timer;

	AFUTIL_EXPORT afutil_timer_handle afutil_timer_create();
	AFUTIL_EXPORT afutil_bool afutil_timer_destroy(afutil_timer_handle handle);
	AFUTIL_EXPORT afutil_bool afutil_timer_register(afutil_timer_handle handle, afutil_timer* timer);
	AFUTIL_EXPORT afutil_bool afutil_timer_unregister(afutil_timer_handle handle, afutil_timer* timer);
#ifdef __cplusplus
}
#endif // __cplusplus

#endif // !AFUTIL_TIMER_H
