#pragma once

#ifndef AFUTIL_SHORTCUT_HPP
#define AFUTIL_SHORTCUT_HPP

#include "afutil_shortcut.h"

#ifdef __cplusplus

#include <string>

namespace afutil
{
	inline bool shortcut_create(std::string source_file, std::string shortcut, std::string description, std::string working_dir, std::string icon_path, int icon_id = 0)
	{
		return afutil_shortcut_create_multibyte(source_file.c_str(), shortcut.c_str(), description.c_str(), working_dir.c_str(), icon_path.c_str(), icon_id);
	}

	inline bool shortcut_create(std::wstring source_file, std::wstring shortcut, std::wstring description, std::wstring working_dir, std::wstring icon_path, int icon_id = 0)
	{
		return afutil_shortcut_create_wide(source_file.c_str(), shortcut.c_str(), description.c_str(), working_dir.c_str(), icon_path.c_str(), icon_id);
	}
}

#endif // __cplusplus

#endif // !AFUTIL_SHORTCUT_HPP
