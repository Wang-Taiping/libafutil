#pragma warning(disable: 4996)

#include "afutil_path.h"
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <filesystem>

namespace fs = std::filesystem;

static inline afutil_rcs afutil_native_rcs_multibyte(const char* old_path, const char* new_path)
{
	return afutil_rcs_instead;
}

static inline afutil_rcs afutil_native_rcs_wide(const wchar_t* old_path, const wchar_t* new_path)
{
	return afutil_rcs_instead;
}

static bool afutil_native_path_rename_multibyte(fs::path old_path, fs::path new_path, afutil_rename_callback_multibyte callback)
{
	std::error_code ec;
	if (!fs::exists(old_path, ec)) return false;
	if (fs::is_regular_file(old_path, ec)) {
		if (!fs::exists(new_path.parent_path(), ec)) fs::create_directories(new_path.parent_path());
		if (fs::exists(new_path, ec)) {
			afutil_rcs rcs = callback(old_path.string().c_str(), new_path.string().c_str());
			switch (rcs)
			{
			case afutil_rcs_cancel:
				return false;
			case afutil_rcs_instead:
				fs::remove_all(new_path, ec);
				fs::rename(old_path, new_path, ec);
				break;
			case afutil_rcs_save:
				return true;
			default:
				return false;
			}
		}
		else {
			fs::rename(old_path, new_path, ec);
		}
	}
	else if (fs::is_directory(old_path, ec)) {
		if (!fs::exists(new_path, ec)) {
			fs::rename(old_path, new_path, ec);
			return !bool(ec);
		}
		if (fs::is_regular_file(new_path, ec)) {
			afutil_rcs rcs = callback(old_path.string().c_str(), new_path.string().c_str());
			switch (rcs)
			{
			case afutil_rcs_cancel:
				return false;
			case afutil_rcs_instead:
				break;
			case afutil_rcs_save:
				fs::remove_all(old_path, ec);
				return true;
			default:
				return false;
			}
			fs::remove(new_path, ec);
			fs::create_directories(new_path, ec);
		}
		for (auto& i : fs::directory_iterator(old_path)) {
			if (!afutil_native_path_rename_multibyte(i.path(), new_path / i.path().filename(), callback))
				return false;
		}
		fs::remove_all(old_path, ec);
	}
	return true;
}

static bool afutil_native_path_rename_wide(fs::path old_path, fs::path new_path, afutil_rename_callback_wide callback)
{
	std::error_code ec;
	if (!fs::exists(old_path, ec)) return false;
	if (fs::is_regular_file(old_path, ec)) {
		if (!fs::exists(new_path.parent_path(), ec)) fs::create_directories(new_path.parent_path());
		if (fs::exists(new_path, ec)) {
			afutil_rcs rcs = callback(old_path.wstring().c_str(), new_path.wstring().c_str());
			switch (rcs)
			{
			case afutil_rcs_cancel:
				return false;
			case afutil_rcs_instead:
				fs::remove_all(new_path, ec);
				fs::rename(old_path, new_path, ec);
				break;
			case afutil_rcs_save:
				return true;
			default:
				return false;
			}
		}
		else {
			fs::rename(old_path, new_path, ec);
		}
	}
	else if (fs::is_directory(old_path, ec)) {
		if (!fs::exists(new_path, ec)) {
			fs::rename(old_path, new_path, ec);
			return !bool(ec);
		}
		if (fs::is_regular_file(new_path, ec)) {
			afutil_rcs rcs = callback(old_path.wstring().c_str(), new_path.wstring().c_str());
			switch (rcs)
			{
			case afutil_rcs_cancel:
				return false;
			case afutil_rcs_instead:
				break;
			case afutil_rcs_save:
				fs::remove_all(old_path, ec);
				return true;
			default:
				return false;
			}
			fs::remove(new_path, ec);
			fs::create_directories(new_path, ec);
		}
		for (auto& i : fs::directory_iterator(old_path)) {
			if (!afutil_native_path_rename_wide(i.path(), new_path / i.path().filename(), callback))
				return false;
		}
		fs::remove_all(old_path, ec);
	}
	return true;
}

int afutil_path_local_file_multibyte(afutil_handle handle, char* buffer, uint32_t bufelemsize)
{
	char* path_str = new char[2048];
	GetModuleFileNameA(HMODULE(handle), path_str, 2048);
	if (GetLastError() == ERROR_INSUFFICIENT_BUFFER) return 0;
	fs::path path = fs::absolute(path_str);
	delete[] path_str;
	if (bufelemsize <= path.string().size()) return static_cast<int>(path.string().size() + 1);
	strcpy(buffer, path.string().c_str());
	return static_cast<int>(path.string().size() + 1);
}

int afutil_path_local_file_wide(afutil_handle handle, wchar_t* buffer, uint32_t bufelemsize)
{
	wchar_t* path_str = new wchar_t[2048];
	GetModuleFileNameW(HMODULE(handle), path_str, 2048);
	if (GetLastError() == ERROR_INSUFFICIENT_BUFFER) return 0;
	fs::path path = fs::absolute(path_str);
	delete[] path_str;
	if (bufelemsize <= path.wstring().size()) return static_cast<int>(path.wstring().size() + 1);
	wcscpy(buffer, path.wstring().c_str());
	return static_cast<int>(path.wstring().size() + 1);
}

int afutil_path_local_dir_multibyte(afutil_handle handle, char* buffer, uint32_t bufelemsize)
{
	char* path_str = new char[2048];
	GetModuleFileNameA(HMODULE(handle), path_str, 2048);
	if (GetLastError() == ERROR_INSUFFICIENT_BUFFER) return 0;
	fs::path path = fs::absolute(path_str).parent_path();
	delete[] path_str;
	if (bufelemsize <= path.string().size()) return static_cast<int>(path.string().size() + 1);
	strcpy(buffer, path.string().c_str());
	return static_cast<int>(path.string().size() + 1);
}

int afutil_path_local_dir_wide(afutil_handle handle, wchar_t* buffer, uint32_t bufelemsize)
{
	wchar_t* path_str = new wchar_t[2048];
	GetModuleFileNameW(HMODULE(handle), path_str, 2048);
	if (GetLastError() == ERROR_INSUFFICIENT_BUFFER) return 0;
	fs::path path = fs::absolute(path_str).parent_path();
	delete[] path_str;
	if (bufelemsize <= path.wstring().size()) return static_cast<int>(path.wstring().size() + 1);
	wcscpy(buffer, path.wstring().c_str());
	return static_cast<int>(path.wstring().size() + 1);
}

int afutil_path_temporary_multibyte(char* buffer, uint32_t bufelemsize)
{
	char* path_str = new char[2048];
	int need_size = GetEnvironmentVariableA("TEMP", path_str, 2048);
	if (GetLastError() == ERROR_ENVVAR_NOT_FOUND || need_size > 2048) return 0;
	fs::path path = fs::absolute(path_str);
	delete[] path_str;
	if (bufelemsize <= path.string().size()) return static_cast<int>(path.string().size() + 1);
	strcpy(buffer, path.string().c_str());
	return static_cast<int>(path.string().size() + 1);
}

int afutil_path_temporary_wide(wchar_t* buffer, uint32_t bufelemsize)
{
	wchar_t* path_str = new wchar_t[2048];
	int need_size = GetEnvironmentVariableW(L"TEMP", path_str, 2048);
	if (GetLastError() == ERROR_ENVVAR_NOT_FOUND || need_size > 2048) return 0;
	fs::path path = fs::absolute(path_str);
	delete[] path_str;
	if (bufelemsize <= path.wstring().size()) return static_cast<int>(path.wstring().size() + 1);
	wcscpy(buffer, path.wstring().c_str());
	return static_cast<int>(path.wstring().size() + 1);
}

afutil_bool afutil_path_rename_multibyte(const char* old_path, const char* new_path)
{
	return afutil_native_path_rename_multibyte(old_path, new_path, afutil_native_rcs_multibyte);
}

afutil_bool afutil_path_rename_wide(const wchar_t* old_path, const wchar_t* new_path)
{
	return afutil_native_path_rename_wide(old_path, new_path, afutil_native_rcs_wide);
}

afutil_bool afutil_path_rename_adv_multibyte(const char* old_path, const char* new_path, afutil_rename_callback_multibyte callback)
{
	return afutil_bool();
}

afutil_bool afutil_path_rename_adv_wide(const wchar_t* old_path, const wchar_t* new_path, afutil_rename_callback_wide callback)
{
	return afutil_bool();
}
