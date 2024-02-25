#include "afutil_shortcut.h"
#include "afutil_string.hpp"
#include <ShlObj.h>

afutil_bool afutil_shortcut_create_multibyte(const char* source_file, const char* shortcut, const char* description, const char* working_dir, const char* icon_path, int icon_id)
{
	// Create buffer.
	std::wstring wsource_file = afutil::utf8_gbk::convert_string(source_file);
	std::wstring wshortcut = afutil::utf8_gbk::convert_string(shortcut);
	std::wstring wdescription;
	std::wstring wworking_dir;
	std::wstring wicon_path;
	// Convert string.
	if (description) wdescription = afutil::utf8_gbk::convert_string(description);
	if (working_dir) wworking_dir = afutil::utf8_gbk::convert_string(working_dir);
	if (icon_path) wicon_path = afutil::utf8_gbk::convert_string(icon_path);
	// Create shortcut and return.
	return afutil_shortcut_create_wide(
		wsource_file.c_str(),
		wshortcut.c_str(),
		(wdescription.empty()) ? nullptr : wdescription.c_str(),
		(wworking_dir.empty()) ? nullptr : wworking_dir.c_str(),
		(wicon_path.empty()) ? nullptr : wicon_path.c_str(),
		icon_id);
}

afutil_bool afutil_shortcut_create_wide(const wchar_t* source_file, const wchar_t* shortcut, const wchar_t* description, const wchar_t* working_dir, const wchar_t* icon_path, int icon_id)
{
	int nReturn = 1;
	IShellLink* pIShellLink = nullptr;
	IPersistFile* pIPersistFile = nullptr;
	try
	{
		HRESULT hr = ::CoCreateInstance(CLSID_ShellLink, nullptr, CLSCTX_INPROC_SERVER,
			IID_IShellLink, (void**)&pIShellLink);
		if (!SUCCEEDED(hr))
			throw "create shell_link object fail.";
		hr = pIShellLink->SetPath(source_file);
		if (!SUCCEEDED(hr))
			throw "set target fail";
		if (description)
		{
			hr = pIShellLink->SetDescription(description);
			if (!SUCCEEDED(hr))
				throw "set description fail.";
		}
		if (working_dir)
		{
			hr = pIShellLink->SetWorkingDirectory(working_dir);
			if (!SUCCEEDED(hr))
				throw "set working_directory fail.";
		}
		if (icon_path)
		{
			hr = pIShellLink->SetIconLocation(icon_path, icon_id);
			if (!SUCCEEDED(hr))
				throw "set icon fail.";
		}
		hr = pIShellLink->QueryInterface(IID_IPersistFile, (void**)&pIPersistFile);
		if (!SUCCEEDED(hr))
			throw "create save object fail.";
		hr = pIPersistFile->Save(shortcut, TRUE);
		if (!SUCCEEDED(hr))
			throw "save fail.";
	}
	catch (...) { nReturn = 0; }
	if (pIPersistFile) pIPersistFile->Release();
	if (pIShellLink) pIShellLink->Release();
	return nReturn;
}