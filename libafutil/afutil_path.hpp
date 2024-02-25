#pragma once

#ifndef AFUTIL_PATH_HPP
#define AFUTIL_PATH_HPP

#include "afutil_path.h"

#ifdef __cplusplus

#if _HAS_CXX17

#include <filesystem>

namespace afutil
{
	inline std::filesystem::path local_file(afutil_handle handle = nullptr)
	{
		wchar_t* path_str = new wchar_t[2048];
		std::filesystem::path path;
		if (afutil_path_local_file_wide(handle, path_str, 2048) != 0)
			path = path_str;
		delete[] path_str;
		return path;
	}

	inline std::filesystem::path local_dir(afutil_handle handle = nullptr)
	{
		wchar_t* path_str = new wchar_t[2048];
		std::filesystem::path path;
		if (afutil_path_local_dir_wide(handle, path_str, 2048) != 0)
			path = path_str;
		delete[] path_str;
		return path;
	}

	inline std::filesystem::path temp_path(afutil_handle handle = nullptr)
	{
		wchar_t* path_str = new wchar_t[2048];
		std::filesystem::path path;
		if (afutil_path_temporary_wide(path_str, 2048) != 0)
			path = path_str;
		delete[] path_str;
		return path;
	}

	inline bool rename(std::filesystem::path old_path, std::filesystem::path new_path)
	{
		return afutil_path_rename_wide(old_path.wstring().c_str(), new_path.wstring().c_str());
	}
}

#endif // _HAS_CXX17

#endif // __cplusplus

#endif // !AFUTIL_PATH_HPP
