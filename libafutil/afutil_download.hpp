#pragma once

#ifndef AFUTIL_DOWNLOAD_HPP
#define AFUTIL_DOWNLOAD_HPP

#include "afutil_download.h"

#ifdef __cplusplus

#include <string>

namespace afutil
{
	typedef afutil_download_callback_t download_callback_t;

	inline bool download(std::string url, std::string path, download_callback_t callback = nullptr, void* user_data = nullptr)
	{
		return afutil_download_file_multibyte(url.c_str(), path.c_str(), callback, user_data);
	}

	inline bool download(std::wstring url, std::wstring path, download_callback_t callback = nullptr, void* user_data = nullptr)
	{
		return afutil_download_file_wide(url.c_str(), path.c_str(), callback, user_data);
	}

	inline bool download_api(std::string url, void* buffer, size_t bufsize, size_t* download_size = nullptr)
	{
		return afutil_download_api_multibyte(url.c_str(), buffer, bufsize, download_size);
	}

	inline bool download_api(std::wstring url, void* buffer, size_t bufsize, size_t* download_size = nullptr)
	{
		return afutil_download_api_wide(url.c_str(), buffer, bufsize, download_size);
	}

	inline std::string url_get_filename(std::string url)
	{
		return url.substr(url.find_last_of('/') + 1);
	}

	inline std::wstring url_get_filename(std::wstring url)
	{
		return url.substr(url.find_last_of('/') + 1);
	}
}

#endif // __cplusplus

#endif // !AFUTIL_DOWNLOAD_HPP
