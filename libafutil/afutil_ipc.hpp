#pragma once

#ifndef AFUTIL_IPC_HPP
#define AFUTIL_IPC_HPP

#include "afutil_ipc.h"

#ifdef __cplusplus

#include <iostream>
#include <fstream>

namespace afutil
{
	class pipe_closed : public std::exception
	{
	public:
		pipe_closed() = default;
		~pipe_closed() = default;
		const char* what() const noexcept override
		{
			return "pipe already closed or not set.";
		}
	};

	class pipebuf : public std::streambuf
	{
	public:
		pipebuf()
		{
			gbuf = new char[1024];
			pbuf = new char[1024];
			setg(gbuf, gbuf, gbuf);
			setp(pbuf, pbuf, pbuf + 1024);
			hpipe = nullptr;
		}
		~pipebuf()
		{
			delete[] gbuf;
			delete[] pbuf;
		}
		afutil_pipe set_pipe(afutil_pipe pipe_handle = nullptr)
		{
			afutil_pipe last_pipe = hpipe;
			hpipe = pipe_handle;
			return last_pipe;
		}
	protected:
		int_type overflow(int_type ch = traits_type::eof()) override
		{
			if (hpipe == nullptr) throw pipe_closed();
			char* str = pbase();
			if (afutil_pipe_write(hpipe, pbase(), pptr() - pbase()) == 0)
			{
				setp(pbuf, nullptr, pbuf + 1024);
				throw pipe_closed();
			}
			setp(pbuf, pbuf, pbuf + 1024);
			return 0;
		}
		int_type underflow() override
		{
			if (hpipe == nullptr) throw pipe_closed();
			size_t getsize = afutil_pipe_read(hpipe, gbuf, 1024);
			if (getsize == 0) 
			{
				setg(gbuf, nullptr, gbuf + 1024);
				throw pipe_closed();
			}
			setg(gbuf, gbuf, gbuf + getsize);
			return *gptr();
		}
		int sync() override
		{
			if (hpipe == nullptr) return -1;
			if (pptr() == pbase()) return 0;
			try { overflow(traits_type::eof()); }
			catch (...) { return -1; }
			return 0;
		}
	private:
		char* gbuf;
		char* pbuf;
		afutil_pipe hpipe;
	};

	class ipipe : public std::istream
	{
	public:
		ipipe() : std::istream(nullptr)
		{
			mybuf = new pipebuf;
			hpipe = nullptr;
			init(mybuf);
		}
		~ipipe()
		{
			delete mybuf;
		}
		afutil_pipe create(std::string pipe_name)
		{
			hpipe = afutil_pipe_create_multibyte(pipe_name.c_str(), std::ios::in);
			return hpipe;
		}
		afutil_pipe create(std::wstring pipe_name)
		{
			hpipe = afutil_pipe_create_wide(pipe_name.c_str(), std::ios::in);
			return hpipe;
		}
		void open()
		{
			if (hpipe == nullptr) return;
			if (afutil_pipe_open(hpipe)) 
			{
				mybuf->set_pipe(hpipe);
				clear();
				return;
			}
			setstate(badbit);
		}
		void close()
		{
			if (hpipe == nullptr) return;
			afutil_pipe_close(hpipe);
			mybuf->set_pipe();
			setstate(badbit);
		}
		afutil_pipe connect(std::string pipe_name)
		{
			hpipe = afutil_pipe_connect_multibyte(pipe_name.c_str(), std::ios::in);
			mybuf->set_pipe(hpipe);
			clear();
			return hpipe;
		}
		afutil_pipe connect(std::wstring pipe_name)
		{
			hpipe = afutil_pipe_connect_wide(pipe_name.c_str(), std::ios::in);
			mybuf->set_pipe(hpipe);
			clear();
			return hpipe;
		}
		void destroy()
		{
			if (hpipe == nullptr) return;
			afutil_pipe_destroy(hpipe);
			mybuf->set_pipe();
			setstate(badbit);
		}
	private:
		pipebuf* mybuf;
		afutil_pipe hpipe;
	};

	class opipe : public std::ostream
	{
	public:
		opipe() : std::ostream(nullptr)
		{
			mybuf = new pipebuf;
			hpipe = nullptr;
			init(mybuf);
		}
		~opipe()
		{
			delete mybuf;
		}
		afutil_pipe create(std::string pipe_name)
		{
			hpipe = afutil_pipe_create_multibyte(pipe_name.c_str(), std::ios::out);
			return hpipe;
		}
		afutil_pipe create(std::wstring pipe_name)
		{
			hpipe = afutil_pipe_create_wide(pipe_name.c_str(), std::ios::in);
			return hpipe;
		}
		void open()
		{
			if (hpipe == nullptr) return;
			if (afutil_pipe_open(hpipe)) 
			{
				mybuf->set_pipe(hpipe);
				clear();
				return;
			}
			setstate(badbit);
		}
		void close()
		{
			if (hpipe == nullptr) return;
			afutil_pipe_close(hpipe);
			mybuf->set_pipe();
			setstate(badbit);
		}
		afutil_pipe connect(std::string pipe_name)
		{
			hpipe = afutil_pipe_connect_multibyte(pipe_name.c_str(), std::ios::out);
			mybuf->set_pipe(hpipe);
			clear();
			return hpipe;
		}
		afutil_pipe connect(std::wstring pipe_name)
		{
			hpipe = afutil_pipe_connect_wide(pipe_name.c_str(), std::ios::out);
			mybuf->set_pipe(hpipe);
			clear();
			return hpipe;
		}
		void destroy()
		{
			if (hpipe == nullptr) return;
			afutil_pipe_destroy(hpipe);
			mybuf->set_pipe();
			setstate(badbit);
		}
	private:
		pipebuf* mybuf;
		afutil_pipe hpipe;
	};

	class pipe : public std::iostream
	{
	public:
		pipe() : std::iostream(nullptr)
		{
			mybuf = new pipebuf;
			hpipe = nullptr;
			init(mybuf);
		}
		~pipe()
		{
			delete mybuf;
		}
		afutil_pipe create(std::string pipe_name, int access_mode)
		{
			hpipe = afutil_pipe_create_multibyte(pipe_name.c_str(), access_mode);
			return hpipe;
		}
		afutil_pipe create(std::wstring pipe_name, int access_mode)
		{
			hpipe = afutil_pipe_create_wide(pipe_name.c_str(), access_mode);
			return hpipe;
		}
		void open()
		{
			if (hpipe == nullptr) return;
			if (afutil_pipe_open(hpipe)) 
			{
				mybuf->set_pipe(hpipe);
				clear();
				return;
			}
			setstate(badbit);
		}
		void close()
		{
			if (hpipe == nullptr) return;
			afutil_pipe_close(hpipe);
			mybuf->set_pipe();
			setstate(badbit);
		}
		afutil_pipe connect(std::string pipe_name, int access_mode)
		{
			hpipe = afutil_pipe_connect_multibyte(pipe_name.c_str(), access_mode);
			mybuf->set_pipe(hpipe);
			clear();
			return hpipe;
		}
		afutil_pipe connect(std::wstring pipe_name, int access_mode)
		{
			hpipe = afutil_pipe_connect_wide(pipe_name.c_str(), access_mode);
			mybuf->set_pipe(hpipe);
			clear();
			return hpipe;
		}
		void destroy()
		{
			if (hpipe == nullptr) return;
			afutil_pipe_destroy(hpipe);
			mybuf->set_pipe();
			setstate(badbit);
		}
	private:
		pipebuf* mybuf;
		afutil_pipe hpipe;
	};
}

#endif // __cplusplus

#endif // !AFUTIL_IPC_HPP
