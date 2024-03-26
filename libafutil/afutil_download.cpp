#pragma warning(disable: 4996)

#include "afutil_download.h"
#include "afutil_string.h"
#include <string>
#include <curl/curl.h>
#include <winhttp.h>
#include <io.h>

#define DOWNLOAD_BUFFER_SIZE	(10485760)

static afutil_timer_handle dltimer;

static bool init = false;

static bool afutil_download_native_getproxy(char* buffer, uint32_t buflen)
{
	WINHTTP_CURRENT_USER_IE_PROXY_CONFIG ProxyConfig = { 0 };
	WinHttpGetIEProxyConfigForCurrentUser(&ProxyConfig);
	if (ProxyConfig.lpszProxy && buflen > wcslen(ProxyConfig.lpszProxy)) {
		afutil_convert_string_to_utf8(ProxyConfig.lpszProxy, buffer, buflen);
		return true;
	}
	return false;
}

afutil_bool afutil_download_init()
{
	if (init) return 0;
	init = true;
	dltimer = afutil_timer_create();
	curl_global_init(CURL_GLOBAL_ALL);
	return 1;
}

afutil_bool afutil_download_uninit()
{
	if (!init) return 0;
	init = false;
	afutil_timer_destroy(dltimer);
	curl_global_cleanup();
	return 1;
}

static int afutil_download_native_progress(afutil_download_metadata* metadata, curl_off_t total, curl_off_t downloaded, curl_off_t, curl_off_t)
{
	if (metadata->native.attribute.exit) return -1;
	metadata->info.total = total;
	metadata->info.downloaded = downloaded;
	if (metadata->native.timer.active_signal) {
		metadata->native.timer.active_signal = false;
		metadata->info.speed = downloaded - metadata->native.info_last_downloaded;
		metadata->native.info_last_downloaded = downloaded;
		uint64_t rest_time = (metadata->info.speed == 0) ? uint64_t(-1) : (total - downloaded) / metadata->info.speed;
		metadata->info.eta.day = rest_time / 86400u;
		metadata->info.eta.hour = rest_time % 86400u / 3600u;
		metadata->info.eta.min = rest_time % 86400u % 3600u / 60u;
		metadata->info.eta.sec = rest_time % 86400u % 3600u % 60u;
		if (metadata->info.speed < 8192) {
			metadata->native.lsbcount++;
			if (metadata->native.lsbcount >= 10) {
				metadata->native.lsbcount = 0;
				metadata->native.attribute.exit = true;
				metadata->native.attribute.retry = true;
				return -1;
			}
		}
		else {
			metadata->native.lsbcount = 0;
		}
	}
	if (metadata->callback == nullptr) return 0;
	afutil_download_operation operation = metadata->callback(&metadata->info, metadata->user_data);
	switch (operation)
	{
	case afutil_download_operation_continue:
		return 0;
	case afutil_download_operation_pause:
		break;
	}
	metadata->native.attribute.exit = true;
	metadata->native.attribute.fail = true;
	return -1;
}

static void afutil_download_native_flushbuffer(afutil_download_metadata* metadata)
{
	if (metadata->native.attribute.exit) return;
	if (metadata->file == nullptr) {
		metadata->native.attribute.exit = true;
		metadata->native.attribute.fail = true;
		return;
	}
	if (metadata->native.cached == 0) return;
	fwrite(metadata->buffer, 1, metadata->native.cached, metadata->file);
	metadata->native.storaged += metadata->native.cached;
	metadata->native.cached = 0;
}

static size_t afutil_download_native_writedata(char* buffer, size_t size, size_t nitems, afutil_download_metadata* metadata)
{
	size_t datasize = size * nitems;
	if (metadata->native.attribute.exit) {
		return datasize;
	}
	if (datasize > metadata->buflen) {
		metadata->native.attribute.exit = true;
		metadata->native.attribute.fail = true;
		return datasize;
	}
	if (metadata->native.cached > (metadata->buflen - datasize)) {
		afutil_download_native_flushbuffer(metadata);
	}
	memcpy(&(metadata->buffer[metadata->native.cached]), buffer, datasize);
	metadata->native.cached += datasize;
	return datasize;
}

afutil_bool afutil_download(afutil_download_metadata* metadata)
{
	if (!init) return 0;
	metadata->native.timer.loops = uint32_t(-1);
	metadata->native.timer.wait_sec = 1u;
	afutil_timer_register(dltimer, &metadata->native.timer);
	metadata->native.curl = curl_easy_init();
	if (metadata->extended_url != nullptr) curl_easy_setopt(metadata->native.curl, CURLOPT_URL, metadata->extended_url);
	else curl_easy_setopt(metadata->native.curl, CURLOPT_URL, metadata->url);
	curl_easy_setopt(metadata->native.curl, CURLOPT_NOPROGRESS, 0L);
	curl_easy_setopt(metadata->native.curl, CURLOPT_XFERINFODATA, metadata);
	curl_easy_setopt(metadata->native.curl, CURLOPT_XFERINFOFUNCTION, afutil_download_native_progress);
	curl_easy_setopt(metadata->native.curl, CURLOPT_FAILONERROR, 1L);
	curl_easy_setopt(metadata->native.curl, CURLOPT_WRITEDATA, metadata);
	curl_easy_setopt(metadata->native.curl, CURLOPT_WRITEFUNCTION, afutil_download_native_writedata);
	char proxy[64] = { 0 };
	if (afutil_download_native_getproxy(proxy, 64)) {
		//printf("%s\n", proxy);
		curl_easy_setopt(metadata->native.curl, CURLOPT_PROXY, proxy);
		curl_easy_setopt(metadata->native.curl, CURLOPT_PROXYTYPE, CURLPROXY_HTTP);
		//curl_easy_setopt(dlstruct->curl, CURLOPT_HTTPPROXYTUNNEL, 1L);
	}
	int retry_times = 0;
	char range[64] = { 0 };
afutil_download_retry:
	curl_easy_perform(metadata->native.curl);
	if (metadata->native.attribute.retry && retry_times < 10) {
		metadata->native.attribute.exit = false;
		metadata->native.attribute.fail = false;
		metadata->native.attribute.retry = false;
		metadata->native.cached = 0;
		snprintf(range, 64, "%llu-%llu", metadata->native.storaged, metadata->info.total - 1);
		//printf("download retry. range: %s\n", range);
		curl_easy_setopt(metadata->native.curl, CURLOPT_RANGE, range);
		retry_times++;
		goto afutil_download_retry;
	}
	curl_easy_cleanup(metadata->native.curl);
	if (metadata->file != nullptr) afutil_download_native_flushbuffer(metadata);
	if (metadata->file != nullptr) fclose(metadata->file);
	metadata->native.attribute.exit = true;
	afutil_timer_unregister(dltimer, &metadata->native.timer);
	return !(metadata->native.attribute.fail || metadata->native.attribute.retry);
}

afutil_bool afutil_download_file(const char* url, const char* path, afutil_download_callback callback, void* user_data)
{
	FILE* fp = fopen(path, "wb");
	if (fp == nullptr) return 0;
	afutil_download_metadata* metadata = new afutil_download_metadata;
	memset(metadata, 0, sizeof(afutil_download_metadata));
	if (strlen(url) >= AFUTIL_DOWNLOAD_URL_DEFAULT_LENGTH) {
		metadata->extended_url = new char[strlen(url) + 1];
		strcpy(metadata->extended_url, url);
		metadata->extended_url[strlen(url)] = 0;
	}
	else {
		strcpy(metadata->url, url);
		metadata->url[strlen(url)] = 0;
	}
	metadata->file = fp;
	metadata->buffer = new char[DOWNLOAD_BUFFER_SIZE];
	metadata->buflen = DOWNLOAD_BUFFER_SIZE;
	metadata->callback = callback;
	metadata->user_data = user_data;
	afutil_bool ret = afutil_download(metadata);
	if (metadata->extended_url != nullptr) delete[] metadata->extended_url;
	delete metadata;
	return ret;
}

afutil_bool afutil_download_api(const char* url, void* buffer, uint64_t buflen, uint64_t* downloaded)
{
	afutil_download_metadata* metadata = new afutil_download_metadata;
	memset(metadata, 0, sizeof(afutil_download_metadata));
	if (strlen(url) >= AFUTIL_DOWNLOAD_URL_DEFAULT_LENGTH) {
		metadata->extended_url = new char[strlen(url) + 1];
		strcpy(metadata->extended_url, url);
		metadata->extended_url[strlen(url)] = 0;
	}
	else {
		strcpy(metadata->url, url);
		metadata->url[strlen(url)] = 0;
	}
	metadata->buffer = (char*)buffer;
	metadata->buflen = buflen;
	afutil_bool ret = afutil_download(metadata);
	if (downloaded != nullptr) *downloaded = metadata->native.cached;
	if (metadata->extended_url != nullptr) delete[] metadata->extended_url;
	delete metadata;
	return ret;
}
