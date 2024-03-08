#include "afutil_optproc.h"
#include <string>
#include <vector>
#include <regex>

typedef std::vector<std::string> strlist;
typedef std::vector<std::wstring> wstrlist;

typedef struct __afutil_opt_native
{
	strlist* list;
	char** argv;
	int argc;
	int optid;
} *afutil_opt_native;

typedef struct __afutil_wopt_native
{
	wstrlist* list;
	wchar_t** argv;
	int argc;
	int optid;
} *afutil_wopt_native;

static std::string keyname(std::string str)
{
	for (size_t i = 0; i < 2; i++) if (str.front() == '-') str.erase(str.begin());
	size_t pos = str.find('=');
	if (pos != std::string::npos) return str.substr(0, pos);
	return str;
}

static std::string keyval(std::string str)
{
	size_t pos = str.find('=');
	if (pos == str.size()) return std::string();
	if (pos != std::string::npos) return str.substr(pos + 1);
	return std::string();
}

static std::wstring wkeyname(std::wstring str)
{
	for (size_t i = 0; i < 2; i++) if (str.front() == '-') str.erase(str.begin());
	size_t pos = str.find('=');
	if (pos != std::wstring::npos) return str.substr(0, pos);
	return str;
}

static std::wstring wkeyval(std::wstring str)
{
	size_t pos = str.find('=');
	if (pos == str.size()) return std::wstring();
	if (pos != std::wstring::npos) return str.substr(pos + 1);
	return std::wstring();
}

afutil_optman afutil_opt_create()
{
	afutil_opt_native handle = new __afutil_opt_native;
	handle->list = new strlist;
	return afutil_optman(handle);
}

void afutil_opt_destroy(afutil_optman opt_handle)
{
	if (opt_handle == nullptr) return;
	afutil_opt_native handle = afutil_opt_native(opt_handle);
	delete handle->list;
	delete handle;
}

afutil_bool afutil_opt_bind(afutil_optman opt_handle, int argc, char* argv[])
{
	if (opt_handle == nullptr || argv == nullptr) return 0;
	afutil_opt_native handle = afutil_opt_native(opt_handle);
	handle->argc = argc;
	handle->argv = argv;
	handle->optid = 1;
	return 1;
}

int afutil_opt_proc(afutil_optman opt_handle, afutil_optrule* rules)
{
	if (opt_handle == nullptr || rules == nullptr) return 0;
	afutil_opt_native handle = afutil_opt_native(opt_handle);
	if (handle->optid >= handle->argc) return AFUTIL_OVERFLOW;
	std::regex normal("^-{1,2}(\\w|-)+");
	std::regex optional("^-{1,2}(\\w|=|-)+");
	std::string temp;
	uint32_t i = 0, j = 0;
	bool finded = false;
	handle->list->clear();
	if (std::regex_match(handle->argv[handle->optid], normal))
	{
		temp = keyname(handle->argv[handle->optid]);
		while (rules[i].name != nullptr)
		{
			if (temp != rules[i].name)
			{
				i++;
				continue;
			}
			handle->optid++;
			if (rules[i].arg_number > 0)
			{
				while (handle->optid < handle->argc && j < rules[i].arg_number && !std::regex_match(handle->argv[handle->optid], optional))
				{
					handle->list->push_back(handle->argv[handle->optid]);
					j++;
					handle->optid++;
				}
				if (handle->list->size() < rules[i].arg_number)
				{
					handle->list->clear();
					return AFUTIL_NOT_EXIST;
				}
			}
			return rules[i].ret_number;
		}
		if (!finded)
		{
			handle->optid++;
			return AFUTIL_NOT_EXIST;
		}
	}
	else if (std::regex_match(handle->argv[handle->optid], optional))
	{
		temp = keyname(handle->argv[handle->optid]);
		while (rules[i].name != nullptr)
		{
			if (rules[i].arg_number != AFUTIL_OPTIONAL || temp != rules[i].name)
			{
				i++;
				continue;
			}
			temp = keyval(handle->argv[handle->optid]);
			if (!temp.empty()) handle->list->push_back(temp);
			handle->optid++;
			return rules[i].ret_number;
		}
		if (!finded) return AFUTIL_NOT_EXIST;
	}
	else
	{
		handle->list->push_back(handle->argv[handle->optid]);
		handle->optid++;
		return AFUTIL_NORMAL;
	}
	return AFUTIL_OVERFLOW;
}

const char* afutil_opt_result(afutil_optman opt_handle, int32_t index)
{
	if (opt_handle == nullptr) return nullptr;
	afutil_opt_native handle = afutil_opt_native(opt_handle);
	if (index < 0)
	{
		if ((size_t)(0 - index) > handle->list->size())
			return nullptr;
		return (handle->list->end() - (size_t)(0 - index))->c_str();
	}
	else
	{
		if (index >= handle->list->size())
			return nullptr;
		return (handle->list->begin() + index)->c_str();
	}
	return nullptr;
}

afutil_woptman afutil_wopt_create()
{
	afutil_wopt_native handle = new __afutil_wopt_native;
	handle->list = new wstrlist;
	return afutil_woptman(handle);
}

void afutil_wopt_destroy(afutil_woptman opt_handle)
{
	if (opt_handle == nullptr) return;
	afutil_wopt_native handle = afutil_wopt_native(opt_handle);
	delete handle->list;
	delete handle;
}

afutil_bool afutil_wopt_bind(afutil_woptman opt_handle, int argc, wchar_t* argv[])
{
	if (opt_handle == nullptr || argv == nullptr) return 0;
	afutil_wopt_native handle = afutil_wopt_native(opt_handle);
	handle->argc = argc;
	handle->argv = argv;
	handle->optid = 1;
	return 1;
}

int afutil_wopt_proc(afutil_woptman opt_handle, afutil_woptrule* rules)
{
	if (opt_handle == nullptr || rules == nullptr) return 0;
	afutil_wopt_native handle = afutil_wopt_native(opt_handle);
	if (handle->optid >= handle->argc) return AFUTIL_OVERFLOW;
	std::wregex normal(L"^-{1,2}(\\w|-)+");
	std::wregex optional(L"^-{1,2}(\\w|=|-)+");
	std::wstring temp;
	uint32_t i = 0, j = 0;
	bool finded = false;
	handle->list->clear();
	if (std::regex_match(handle->argv[handle->optid], normal))
	{
		temp = wkeyname(handle->argv[handle->optid]);
		while (rules[i].name != nullptr)
		{
			if (temp != rules[i].name)
			{
				i++;
				continue;
			}
			handle->optid++;
			if (rules[i].arg_number > 0)
			{
				while (handle->optid < handle->argc && j < rules[i].arg_number && !std::regex_match(handle->argv[handle->optid], optional))
				{
					handle->list->push_back(handle->argv[handle->optid]);
					j++;
					handle->optid++;
				}
				if (handle->list->size() < rules[i].arg_number)
				{
					handle->list->clear();
					return AFUTIL_NOT_EXIST;
				}
			}
			return rules[i].ret_number;
		}
		if (!finded)
		{
			handle->optid++;
			return AFUTIL_NOT_EXIST;
		}
	}
	else if (std::regex_match(handle->argv[handle->optid], optional))
	{
		temp = wkeyname(handle->argv[handle->optid]);
		while (rules[i].name != nullptr)
		{
			if (rules[i].arg_number != AFUTIL_OPTIONAL || temp != rules[i].name)
			{
				i++;
				continue;
			}
			temp = wkeyval(handle->argv[handle->optid]);
			if (!temp.empty()) handle->list->push_back(temp);
			handle->optid++;
			return rules[i].ret_number;
		}
		if (!finded) return AFUTIL_NOT_EXIST;
	}
	else
	{
		handle->list->push_back(handle->argv[handle->optid]);
		handle->optid++;
		return AFUTIL_NORMAL;
	}
	return AFUTIL_OVERFLOW;
}

const wchar_t* afutil_wopt_result(afutil_woptman opt_handle, int32_t index)
{
	if (opt_handle == nullptr) return nullptr;
	afutil_wopt_native handle = afutil_wopt_native(opt_handle);
	if (index < 0)
	{
		if ((size_t)(0 - index) > handle->list->size())
			return nullptr;
		return (handle->list->end() - (size_t)(0 - index))->c_str();
	}
	else
	{
		if (index >= handle->list->size())
			return nullptr;
		return (handle->list->begin() + index)->c_str();
	}
	return nullptr;
}
