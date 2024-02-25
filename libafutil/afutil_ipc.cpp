#include "afutil_ipc.h"
#include <string>
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#define AFC_PIPE_PREFIXA	R"(\\.\pipe\)"
#define AFC_PIPE_PREFIXW	LR"(\\.\pipe\)"

afutil_bool afutil_pipe_exists_multibyte(const char* pipe_name, uint32_t milliseconds)
{
	std::string name = AFC_PIPE_PREFIXA;
	name += pipe_name;
	while (milliseconds > 200)
	{
		if (WaitNamedPipeA(name.c_str(), 1)) return true;
		Sleep(199);
		milliseconds -= 200;
	}
	Sleep(milliseconds - 1);
	return WaitNamedPipeA(name.c_str(), 1);
}

afutil_bool afutil_pipe_exists_wide(const wchar_t* pipe_name, uint32_t milliseconds)
{
	std::wstring name = AFC_PIPE_PREFIXW;
	name += pipe_name;
	while (milliseconds > 200)
	{
		if (WaitNamedPipeW(name.c_str(), 1)) return true;
		Sleep(199);
		milliseconds -= 200;
	}
	Sleep(milliseconds - 1);
	return WaitNamedPipeW(name.c_str(), 1);
}

afutil_pipe afutil_pipe_create_multibyte(const char* pipe_name, int access_mode)
{
	if (!(access_mode & AFUTIL_PIPE_ACCESS_ALL)) return nullptr;
	std::string name = AFC_PIPE_PREFIXA;
	name += pipe_name;
	afutil_pipe hPipe = (afutil_pipe)CreateNamedPipeA(name.c_str(), DWORD(access_mode) | FILE_FLAG_FIRST_PIPE_INSTANCE, PIPE_WAIT | PIPE_TYPE_BYTE | PIPE_READMODE_BYTE, 1, 4096, 4096, 0, nullptr);
	return (hPipe == INVALID_HANDLE_VALUE) ? nullptr : hPipe;
}

afutil_pipe afutil_pipe_create_wide(const wchar_t* pipe_name, int access_mode)
{
	if (!(access_mode & AFUTIL_PIPE_ACCESS_ALL)) return nullptr;
	std::wstring name = AFC_PIPE_PREFIXW;
	name += pipe_name;
	afutil_pipe hPipe = (afutil_pipe)CreateNamedPipeW(name.c_str(), DWORD(access_mode) | FILE_FLAG_FIRST_PIPE_INSTANCE, PIPE_WAIT | PIPE_TYPE_BYTE | PIPE_READMODE_BYTE, 1, 4096, 4096, 0, nullptr);
	return (hPipe == INVALID_HANDLE_VALUE) ? nullptr : hPipe;
}

afutil_bool afutil_pipe_open(afutil_pipe pipe_handle)
{
	if (pipe_handle == nullptr) return 0;
	return ConnectNamedPipe(pipe_handle, nullptr);
}

void afutil_pipe_close(afutil_pipe pipe_handle)
{
	if (pipe_handle == nullptr) return;
	DisconnectNamedPipe(pipe_handle);
}

afutil_pipe afutil_pipe_connect_multibyte(const char* pipe_name, int access_mode)
{
	if (!(access_mode & AFUTIL_PIPE_ACCESS_ALL)) return nullptr;
	std::string name = AFC_PIPE_PREFIXA;
	name += pipe_name;
	DWORD Permission = 0;
	if (access_mode & AFUTIL_PIPE_ACCESS_READ) Permission |= GENERIC_READ;
	if (access_mode & AFUTIL_PIPE_ACCESS_WRITE) Permission |= GENERIC_WRITE;
	afutil_pipe hPipe = (afutil_pipe)CreateFileA(name.c_str(), Permission, 0, nullptr, OPEN_EXISTING, 0, nullptr);
	return (hPipe == INVALID_HANDLE_VALUE) ? nullptr : hPipe;
}

afutil_pipe afutil_pipe_connect_wide(const wchar_t* pipe_name, int access_mode)
{
	if (!(access_mode & AFUTIL_PIPE_ACCESS_ALL)) return nullptr;
	std::wstring name = AFC_PIPE_PREFIXW;
	name += pipe_name;
	DWORD Permission = 0;
	if (access_mode & AFUTIL_PIPE_ACCESS_READ) Permission |= GENERIC_READ;
	if (access_mode & AFUTIL_PIPE_ACCESS_WRITE) Permission |= GENERIC_WRITE;
	afutil_pipe hPipe = (afutil_pipe)CreateFileW(name.c_str(), Permission, 0, nullptr, OPEN_EXISTING, 0, nullptr);
	return (hPipe == INVALID_HANDLE_VALUE) ? nullptr : hPipe;
}

void afutil_pipe_destroy(afutil_pipe pipe_handle)
{
	if (pipe_handle == nullptr) return;
	CloseHandle(pipe_handle);
}

uint32_t afutil_pipe_read(afutil_pipe pipe_handle, void* buffer, uint32_t read_size)
{
	if (pipe_handle == nullptr || buffer == nullptr || read_size == 0) return 0;
	DWORD readed = 0;
	if (!ReadFile(pipe_handle, buffer, read_size, &readed, nullptr))
		return 0;
	return readed;
}

uint32_t afutil_pipe_write(afutil_pipe pipe_handle, const void* data, uint32_t write_size)
{
	if (pipe_handle == nullptr || data == nullptr || write_size == 0) return 0;
	DWORD writed = 0;
	if (!WriteFile(pipe_handle, data, write_size, &writed, nullptr))
		return 0;
	return writed;
}
