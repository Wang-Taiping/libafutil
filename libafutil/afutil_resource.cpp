#include "afutil_resource.h"
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

size_t afutil_resource_load_multibyte(void* hmodule, const char* type, int rscid, void** buffer)
{
	HRSRC hRSrc = FindResourceA(HMODULE(hmodule), MAKEINTRESOURCEA(rscid), type);
	if (!hRSrc)
		return 0;
	HGLOBAL hGlobal = LoadResource(HMODULE(hmodule), hRSrc);
	if (!hGlobal)
		return 0;
	LPVOID Buffer = LockResource(hGlobal);
	if (!Buffer)
		return 0;
	DWORD nBufLen = SizeofResource(HMODULE(hmodule), hRSrc);
	*buffer = Buffer;
	return nBufLen;
}

size_t afutil_resource_load_wide(void* hmodule, const wchar_t* type, int rscid, void** buffer)
{
	HRSRC hRSrc = FindResourceW(HMODULE(hmodule), MAKEINTRESOURCEW(rscid), type);
	if (!hRSrc)
		return 0;
	HGLOBAL hGlobal = LoadResource(HMODULE(hmodule), hRSrc);
	if (!hGlobal)
		return 0;
	LPVOID Buffer = LockResource(hGlobal);
	if (!Buffer)
		return 0;
	DWORD nBufLen = SizeofResource(HMODULE(hmodule), hRSrc);
	*buffer = Buffer;
	return nBufLen;
}
