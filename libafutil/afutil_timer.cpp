#include "afutil_timer.h"
#include <thread>
#include <vector>
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

static std::vector<afutil_timer*> timer_list;
static bool timer_exit = true;
static bool timer_scanning = false;

static void afutil_timer_native_thread();

afutil_bool afutil_timer_init()
{
	if (!timer_exit) return false;
	timer_list.clear();
	timer_exit = false;
	std::thread(afutil_timer_native_thread).detach();
	return true;
}

afutil_bool afutil_timer_uninit()
{
	if (timer_exit) return false;
	timer_list.clear();
	timer_exit = true;
	return true;
}

afutil_bool afutil_timer_register(afutil_timer* timer)
{
	while (timer_scanning) Sleep(50);
	for (auto& i : timer_list) {
		if (i == timer) return 0;
	}
	timer_list.push_back(timer);
	return 1;
}

afutil_bool afutil_timer_unregister(afutil_timer* timer)
{
	while (timer_scanning) Sleep(50);
	bool finded = false;
	for (auto i = timer_list.begin(); i != timer_list.end();) {
		if (*i == timer) {
			finded = true;
			i = timer_list.erase(i);
		}
		else {
			++i;
		}
	}
	return finded ? 1 : 0;
}

void afutil_timer_native_thread()
{
	while (!timer_exit) {
		Sleep(1000);
		timer_scanning = true;
		for (auto i = timer_list.begin(); i != timer_list.end();) {
			(*i)->expired_sec++;
			if ((*i)->expired_sec == (*i)->wait_sec) {
				(*i)->expired_sec = 0;
				(*i)->loops--;
				(*i)->active_signal = true;
				if ((*i)->callback) (*i)->callback((*i)->user_data);
			}
			if ((*i)->loops == 0) {
				i = timer_list.erase(i);
			}
			else {
				++i;
			}
		}
		timer_scanning = false;
	}
}
