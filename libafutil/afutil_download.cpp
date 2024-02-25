#pragma warning(disable: 4996)

#include "afutil_download.h"
#include "afutil_string.h"
#include <string>
#include <curl/curl.h>

constexpr auto DEFAULT_BUFFER_SIZE = (4 * 1024 * 1024);

struct afutil_dlstruct
{
	CURL* curl;
	afutil_download_callback_t callback;
	void* user_data;
	char* buffer;
	size_t bufsize;
	size_t bufcache;
	FILE* file;
	bool error;
};

static int progress(afutil_dlstruct* dlstruct, curl_off_t dltotal, curl_off_t dlnow, curl_off_t, curl_off_t)
{
	if (dlstruct->error) return -1;
	if (dlstruct->callback == nullptr) return 0;
	curl_off_t dlspeed = 0;
	curl_easy_getinfo(dlstruct->curl, CURLINFO_SPEED_DOWNLOAD_T, &dlspeed);
	return dlstruct->callback(dltotal, dlnow, dlspeed, dlstruct->user_data) ? 0 : -1;
}

static void flushbuffer(afutil_dlstruct* dlstruct)
{
	if (dlstruct->error) return;
	if (dlstruct->file == nullptr)
	{
		dlstruct->error = true;
		return;
	}
	if (dlstruct->bufcache == 0) return;
	fwrite(dlstruct->buffer, 1, dlstruct->bufcache, dlstruct->file);
	dlstruct->bufcache = 0;
}

static size_t writedata(char* buffer, size_t size, size_t nitems, afutil_dlstruct* dlstruct)
{
	size_t datasize = size * nitems;
	if (dlstruct->error) return datasize;
	if (datasize > dlstruct->bufsize)
	{
		dlstruct->error = true;
		return datasize;
	}
	if (dlstruct->bufcache > (dlstruct->bufsize - datasize))
	{
		if (dlstruct->file == nullptr)
		{
			dlstruct->error = true;
			return datasize;
		}
		else flushbuffer(dlstruct);
	}
	memcpy(&(dlstruct->buffer[dlstruct->bufcache]), buffer, datasize);
	dlstruct->bufcache += datasize;
	return datasize;
}

static int download(const char* url, afutil_dlstruct* dlstruct)
{
	dlstruct->curl = curl_easy_init();
	curl_easy_setopt(dlstruct->curl, CURLOPT_URL, url);
	curl_easy_setopt(dlstruct->curl, CURLOPT_NOPROGRESS, 0L);
	curl_easy_setopt(dlstruct->curl, CURLOPT_XFERINFODATA, dlstruct);
	curl_easy_setopt(dlstruct->curl, CURLOPT_XFERINFOFUNCTION, progress);
	curl_easy_setopt(dlstruct->curl, CURLOPT_FAILONERROR, 1L);
	curl_easy_setopt(dlstruct->curl, CURLOPT_WRITEDATA, dlstruct);
	curl_easy_setopt(dlstruct->curl, CURLOPT_WRITEFUNCTION, writedata);
	CURLcode code = curl_easy_perform(dlstruct->curl);
	curl_easy_cleanup(dlstruct->curl);
	return code == CURLE_OK;
}

afutil_bool afutil_download_file_multibyte(const char* url, const char* path, afutil_download_callback_t callback, void* user_data)
{
	afutil_dlstruct dlstruct = { 0 };
	dlstruct.file = fopen(path, "wb");
	if (dlstruct.file == nullptr) return 0;
	dlstruct.callback = callback;
	dlstruct.user_data = user_data;
	dlstruct.error = false;
	dlstruct.buffer = new char[DEFAULT_BUFFER_SIZE];
	dlstruct.bufsize = DEFAULT_BUFFER_SIZE;
	int ret = download(url, &dlstruct);
	flushbuffer(&dlstruct);
	delete[] dlstruct.buffer;
	fclose(dlstruct.file);
	return ret;
}

afutil_bool afutil_download_file_wide(const wchar_t* url, const wchar_t* path, afutil_download_callback_t callback, void* user_data)
{
	int urlsize = afutil_convert_string_to_utf8(url, nullptr, 0);
	int pathsize = afutil_convert_string_to_utf8(path, nullptr, 0);
	char* _url = new char[urlsize];
	char* _path = new char[pathsize];
	memset(_url, 0, urlsize);
	memset(_path, 0, pathsize);
	afutil_convert_string_to_utf8(url, _url, urlsize);
	afutil_convert_string_to_utf8(path, _path, pathsize);
	int ret = afutil_download_file_multibyte(_url, _path, callback);
	delete[] _url;
	delete[] _path;
	return ret;
}

afutil_bool afutil_download_api_multibyte(const char* url, void* buffer, size_t bufsize, size_t* download_size)
{
	afutil_dlstruct dlstruct = { 0 };
	dlstruct.error = false;
	dlstruct.buffer = (char*)buffer;
	dlstruct.bufsize = bufsize;
	int ret = download(url, &dlstruct);
	if (download_size != nullptr) *download_size = dlstruct.bufcache;
	return ret;
}

afutil_bool afutil_download_api_wide(const wchar_t* url, void* buffer, size_t bufsize, size_t* download_size)
{
	int urlsize = afutil_convert_string_to_utf8(url, nullptr, 0);
	char* _url = new char[urlsize];
	memset(_url, 0, urlsize);
	afutil_convert_string_to_utf8(url, _url, urlsize);
	int ret = afutil_download_api_multibyte(_url, buffer, bufsize, download_size);
	delete[] _url;
	return ret;
}

static std::string urlname(std::string url)
{
	return url.substr(url.find_last_of('/') + 1);
}

static std::wstring urlname(std::wstring url)
{
	return url.substr(url.find_last_of('/') + 1);
}

size_t afutil_url_get_filename_multibyte(const char* url, char* buffer, size_t bufsize)
{
	std::string temp = urlname(url);
	if (bufsize <= temp.size()) return temp.size() + 1;
	strcpy(buffer, temp.c_str());
	buffer[temp.size()] = 0;
	return temp.size() + 1;
}

size_t afutil_url_get_filename_wide(const wchar_t* url, wchar_t* buffer, size_t bufsize)
{
	std::wstring temp = urlname(url);
	if (bufsize <= temp.size()) return temp.size() + 1;
	wcscpy(buffer, temp.c_str());
	buffer[temp.size()] = 0;
	return temp.size() + 1;
}
