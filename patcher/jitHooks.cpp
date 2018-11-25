#include "jitHooks.hpp"

vector<char*> encoded_instr;
char* filename = "./hooks/hook.h";

bool hexToU64(uint64_t& out, const char* src, size_t len)
{
	uint64_t val = 0;
	for (size_t i = 0; i < len; i++)
	{
		uint32_t c = src[i];
		if (c >= '0' && c <= '9')
			c = c - '0';
		else if (c >= 'a' && c <= 'f')
			c = c - 'a' + 10;
		else if (c >= 'A' && c <= 'F')
			c = c - 'A' + 10;
		else
			return false;
		val = (val << 4) | c;
	}

	out = val;
	return true;
}

void dumpCode(CodeBuffer buffer)
{
	size_t _size = buffer.getLength();
	printf("%s","Current hook buffer is:\n");
	for (size_t i = 0; i < _size; i++)
	{
		printf("%02X", buffer.getData()[i]);
	}
}

void saveCode(CodeBuffer buffer, char* filename)
{
	FILE *pFile = fopen (filename, "wb");
	size_t _size = encoded_instr.size();
	char* tok;
	if(!pFile)
	{
		printf("\nCan't open \"%s\".\n",filename);
		printf( "Error code opening file: %d\n", errno );
		printf( "Error opening file: %s\n", strerror( errno ) );
		debug_pause();
	}
	for(size_t i = 0; i < _size; i++)
	{
		fwrite(encoded_instr[i], sizeof(char), ::strlen(encoded_instr[i]), pFile);
	}
	fwrite("BYTES:", sizeof(char), 6, pFile);
	_size = buffer.getLength();
	for (size_t i = 0; i < _size; i++)
	{
		fprintf(pFile, "%02X", buffer.getData()[i]);
	}
	fclose (pFile);
}

bool isSpace(const char c)
{
	return c == ' ' || c == '\r' || c == '\n' || c == '\t';
}

bool isCommand(const char* str, const char* cmd)
{
	while (str[0] && isSpace(str[0])) str++;

	size_t sLen = ::strlen(str);
	while (sLen && isSpace(str[sLen - 1])) sLen--;

	size_t cLen = ::strlen(cmd);
	return sLen == cLen && ::memcmp(str, cmd, sLen) == 0;
}

int enter_asmjit_hook(int argc, char* argv[])
{

	CmdLine cmd(argc, argv);
	const char* archArg = cmd.getKey("--arch");
	const char* baseArg = cmd.getKey("--base");

	uint32_t archType = ArchInfo::kTypeX64;
	uint64_t baseAddress = Globals::kNoBaseAddress;

	if (archArg)
	{
		if (::strcmp(archArg, "x86") == 0)
		{
			archType = ArchInfo::kTypeX86;
		}
		else if (::strcmp(archArg, "x64") == 0)
		{
			archType = ArchInfo::kTypeX64;
		}
		else
		{
			printf("Invalid --arch parameter\n");
			return 1;
		}
	}
	else
	{
		archArg = "x64";
	}

	if (baseArg)
	{
		size_t len = ::strlen(baseArg);
		size_t maxLen = archType == ArchInfo::kTypeX64 ? 16 : 8;

		if (!len || len > maxLen || !hexToU64(baseAddress, baseArg, len))
		{
			printf("Invalid --base parameter\n");
			return 1;
		}
	}

	printf("===============================================================\n");
	printf("Remote Assembler:\n"                      );
	printf("  - A simple command-line based instruction encoder\n"            );
	printf("  - Architecture=%s [select by --arch=x86|x64]\n", archArg        );
	printf("  - Base-Address=%s [select by --base=hex]\n", baseArg            );
	printf("---------------------------------------------------------------\n");
	printf("Input:\n"                                                         );
	printf("  - Enter instruction and its operands to be encoded.\n"          );
	printf("  - Enter '.clear' to clear everything.\n"                        );
	printf("  - Enter '.print' to print the current code.\n"                  );
	printf("  - Enter '.save' to save the current code.\n"                  );
	printf("  - Enter '.undo' to revoke last encoded instruction.\n"          );
	printf("  - Enter '.file' to change filename.\n"          );
	printf("===============================================================\n");

	StringLogger logger;
	logger.addOptions(Logger::kOptionBinaryForm);

	CodeInfo ci(archType, 0, baseAddress);
	CodeHolder code;

	code.init(ci);
	code.setLogger(&logger);

	X86Assembler a(&code);
	AsmParser p(&a);

	char input[4096];
	for (;;)
	{
		fgets(input, 4095, stdin);
		if (input[0] == 0) break;

		if (isCommand(input, ".clear"))
		{
			code.reset(false);  // Detaches everything.
			code.init(ci);
			code.setLogger(&logger);
			code.attach(&a);
			continue;
		}

		if (isCommand(input, ".print"))
		{
			code.sync(); // First sync with the assembler.

			// 0 is the section number, this case .text
			CodeBuffer& buffer = code.getSectionEntry(0)->getBuffer();
			dumpCode(buffer);
			continue;
		}

		if (isCommand(input, ".save"))
		{
			code.sync();
			CodeBuffer& buffer = code.getSectionEntry(0)->getBuffer();
			if(buffer.hasData())
			{
				//printf("%.*s",filename);
				saveCode(buffer,filename);
			}
			continue;
		}

		if (isCommand(input, ".file"))
		{
			printf("Set new filename...\n");
			fgets(input, 4095, stdin);
			filename = input;
			enter_asmjit_hook(argc,argv);
		}

		if (isCommand(input, ".undo"))
		{
			code.sync();
			if(!encoded_instr.empty())
			{
				encoded_instr.pop_back();
			}
			CodeBuffer& buffer = code.getSectionEntry(0)->getBuffer();
			if(buffer.hasData())
			{
				//to do
			}
			continue;
		}

		logger.clearString();
		Error err = p.parse(input);

		if (err == kErrorOk)
		{
			char* log = logger.getString();
			size_t i;
			size_t len = logger.getLength();

			// Skip the instruction part, and keep only the comment part.
			for (i = 0; i < len; i++)
			{
				if (log[i] == ';')
				{
					i += 2;
					break;
				}
			}

			if (i < len)
			{
				encoded_instr.push_back(log);
				printf("%.*s", (int)(len - i), log + i);
			}
		}
		else
		{
			a.resetLastError();
			fprintf(stdout, "ERROR: 0x%08X: %s\n", err, DebugUtils::errorAsString(err));
		}
	}

	return 0;
}