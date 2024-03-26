#include "afutil_timer.h"
#include <thread>
#include <vector>
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

struct afutil_timer_native
{
	std::vector<afutil_timer*> timer_list;
	bool timer_exit;
	bool timer_scanning;
};

static void afutil_timer_native_thread(afutil_timer_native* nhandle);

afutil_timer_handle afutil_timer_create()
{
	afutil_timer_native* nhandle = new afutil_timer_native;
	nhandle->timer_list.clear();
	nhandle->timer_exit = false;
	nhandle->timer_scanning = false;
	std::thread(afutil_timer_native_thread, nhandle).detach();
	return afutil_timer_handle(nhandle);
}

afutil_bool afutil_timer_destroy(afutil_timer_handle handle)
{
	afutil_timer_native* nhandle = (afutil_timer_native*)handle;
	if (nhandle->timer_exit) return false;
	nhandle->timer_list.clear();
	nhandle->timer_exit = true;
	return 1;
}

afutil_bool afutil_timer_register(afutil_timer_handle handle, afutil_timer* timer)
{
	afutil_timer_native* nhandle = (afutil_timer_native*)handle;
	while (nhandle->timer_scanning) Sleep(50);
	for (auto& i : nhandle->timer_list) {
		if (i == timer) return 0;
	}
	nhandle->timer_list.push_back(timer);
	return 1;
}

afutil_bool afutil_timer_unregister(afutil_timer_handle handle, afutil_timer* timer)
{
	afutil_timer_native* nhandle = (afutil_timer_native*)handle;
	while (nhandle->timer_scanning) Sleep(50);
	bool finded = false;
	for (auto i = nhandle->timer_list.begin(); i != nhandle->timer_list.end();) {
		if (*i == timer) {
			finded = true;
			i = nhandle->timer_list.erase(i);
		}
		else {
			++i;
		}
	}
	return finded ? 1 : 0;
}

void afutil_timer_native_thread(afutil_timer_native* nhandle)
{
	while (!nhandle->timer_exit) {
		Sleep(1000);
		nhandle->timer_scanning = true;
		for (auto i = nhandle->timer_list.begin(); i != nhandle->timer_list.end();) {
			(*i)->expired_sec++;
			if ((*i)->expired_sec == (*i)->wait_sec) {
				(*i)->expired_sec = 0;
				(*i)->loops--;
				(*i)->active_signal = true;
				if ((*i)->callback) (*i)->callback((*i)->user_data);
			}
			if ((*i)->loops == 0) {
				i = nhandle->timer_list.erase(i);
			}
			else {
				++i;
			}
		}
		nhandle->timer_scanning = false;
	}
	delete nhandle;
}
