#pragma once
#include "patcher.hpp"
#include <asmjit_lib/asmjit.h>
#include <asmjit_lib/x86.h>
#include <asmjit_lib/asmtk/asmtk.h>

using namespace asmjit;
using namespace asmtk;

bool hexToU64(uint64_t& out, const char* src, size_t len);
void dumpCode(CodeBuffer buffer);
void saveCode(CodeBuffer buffer, char* filename = "./hooks/hook.h");
bool isSpace(const char c);
bool isCommand(const char* str, const char* cmd);
int enter_asmjit_hook(int argc, char* argv[]);

class CmdLine
{
public:
	CmdLine(int argc, const char* const* argv)
		: argc(argc),
		  argv(argv) {}

	bool hasKey(const char* key) const
	{
		for (int i = 0; i < argc; i++)
			if (::strcmp(argv[i], key) == 0)
				return true;
		return false;
	}

	const char* getKey(const char* key) const
	{
		size_t keyLen = ::strlen(key);
		size_t argLen = 0;

		const char* arg = NULL;
		for (int i = 0; i <= argc; i++)
		{
			if (i == argc)
				return NULL;

			arg = argv[i];
			argLen = ::strlen(arg);
			if (argLen >= keyLen && ::memcmp(arg, key, keyLen) == 0)
				break;
		}

		if (argLen > keyLen && arg[keyLen] == '=')
			return arg + keyLen + 1;
		else
			return arg + keyLen;
	}

	int argc;
	const char* const* argv;
};