#include "jitHooks.hpp"

inline size_t fstrlen (const char* s)
{
	register const char* i;
	for(i=s; *i; ++i);
	return (i-s);
}

bool hexToU64(uint64_t& out, const char* src, size_t len)
{
	uint64_t val = 0;
	int null_count = 0;

	for(size_t i=0; i<=len; i++)
	{
		if(src[i]=='\0' || src[i]==' ')
		{
			null_count++;
		}
	}
	len = len-null_count;
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

int getposition(const char *array, size_t size, char c)
{
	for (size_t i = 0; i < size; i++)
	{
		if (array[i] == c)
			return (int)i;
	}
	return -1;
}

bool isSpace(const char c)
{
	return c == ' ' || c == '\r' || c == '\n' || c == '\t';
}

bool isCommand(const char* str, const char* cmd)
{
	while (str[0] && isSpace(str[0])) str++;

	size_t sLen = fstrlen(str);
	while (sLen && isSpace(str[sLen - 1])) sLen--;

	size_t cLen = fstrlen(cmd);
	return sLen == cLen && ::memcmp(str, cmd, sLen) == 0;
}

vector<char> HexToBytes(const string& hex)
{
	vector<char> bytes;

	for (unsigned int i = 0; i < hex.length(); i += 2)
	{
		string byteString = hex.substr(i, 2);
		char byte = (char) strtol(byteString.c_str(), nullptr, 16);
		bytes.push_back(byte);
	}

	return bytes;
}

//Non reusable functions: Aimed for one task...

#define MAX_MEMORY_SIZE 4095

vector<char*> encoded_instr;
string buffer_from_file_only;
string filename = "./hooks/jithook.jh";
unsigned int address_inc = 3;
char* archArg = nullptr;
char* baseArg = nullptr;
char* offsArg = nullptr;
char* arg_memblock = nullptr;
char* instr_memblock = nullptr;
bool dispatch = true;
int instr_memblock_offset = 0;
size_t layer = 0;
uint32_t archType;
uint64_t baseAddress;
uint64_t baseOffset;

typedef struct asmjit_objs
{
	StringLogger *logger;
	CodeInfo *ci;
	CodeHolder *code;
	X86Assembler *a;
	AsmParser *p;
	CodeBuffer *buffer;
} ptr;

static ptr *asmjit_state;

static void dumpCode(CodeBuffer buffer, size_t _size)
{
	printf("%s","Current hook buffer is:\n");
	for (size_t i = 0; i < _size; i++)
	{
		printf("%02X", buffer.getData()[i]);
	}
	printf(&buffer_from_file_only[0]);
}

static void saveCode(CodeBuffer buffer, char* _filename, uint64_t baseAddress)
{
	FILE *pFile = fopen (_filename, "w");
	size_t _size = encoded_instr.size();
	if(!pFile)
	{
		printf("\nCan't open \"%s\".\n",_filename);
		printf( "Error code opening file: %d\n", errno );
		printf( "Error opening file: %s\n", strerror( errno ) );
		debug_pause();
	}
	fprintf(pFile, "--baseVA=%04X \n", baseAddress);
	fprintf(pFile, "--offs=%04X \n", baseOffset);
	fprintf(pFile, "--arch=%s \n", archArg);
	fprintf(pFile, "; Start\n");
	for(size_t i = 0; i < _size; i++)
	{
		fwrite(encoded_instr[i], sizeof(char), fstrlen(encoded_instr[i]), pFile);
	}
	fprintf(pFile, "; Finish\n");
	fwrite("BYTES:", sizeof(char), 6, pFile);
	fwrite(&buffer_from_file_only[0], sizeof(char), buffer_from_file_only.length(), pFile);
	_size = buffer.getLength();
	for (size_t i = 0; i < _size; i++)
	{
		fprintf(pFile, "%02X", buffer.getData()[i]);
	}
	fclose (pFile);
}

inline void reset_globals(void)
{
	encoded_instr.clear();
	buffer_from_file_only.clear();
	archArg = nullptr;
	baseArg = nullptr;
	offsArg = nullptr;
	arg_memblock = nullptr;
	baseOffset = 0;
}

inline void mark_mem(int offset)
{
	instr_memblock_offset += offset;
	if(instr_memblock_offset >= MAX_MEMORY_SIZE)
	{
		instr_memblock = realloc(instr_memblock, instr_memblock_offset * 2);
	}
}

char* interpret_instr(char* instr, int align_calls=0x1000)
{
	return instr;
}

static void compile_asm(FILE* pFile)
{
	const int line_size = 4096;
	bool start = false;
	char line[line_size];
	char* pos;
	int len;

try_parse:

	while(fgets(&line[0], line_size, pFile) != nullptr)
	{
		if(start||strncmp(&line[0],"; Start",7)==0)
		{
			len = fstrlen(&line[0]);
			if(!start)
			{
				start = true;
				continue;
			}
			if(strncmp(&line[0],"; Finish",8)==0)
			{
				break;
			}
			pos = strstr(&line[0], ";");
			if(pos != 0)
			{
				//remove the comments
				memset(pos, 0, pos-&line[0]+len);
				pos = '\0';
			}

			asmjit_state->buffer = &asmjit_state->code->getSectionEntry(0)->getBuffer();
			asmjit_state->logger->clearString();
			Error err = asmjit_state->p->parse(line);

			if (err == kErrorOk)
			{
				char* log = asmjit_state->logger->getString();
				int length = asmjit_state->logger->getLength();
				int i;

				// Skip the instruction part, and keep only the comment part.
				for (i = 0; i < length; i++)
				{
					if (log[i] == ';')
					{
						i += 2;
						break;
					}
				}

				if (i < length)
				{
					int step_bytes_length = length - i;
					char temp1[step_bytes_length];
					sprintf(&temp1[0], "%.*s", (int)(step_bytes_length), log + i);
					printf("%s", &temp1[0]);

					mark_mem(length * sizeof(char*));
					strncpy(instr_memblock+instr_memblock_offset, log, length+1);
					instr_memblock[(instr_memblock_offset/sizeof(char*))+length] = '\0';
					encoded_instr.push_back(instr_memblock+instr_memblock_offset);

					asmjit_state->ci->_baseAddress = asmjit_state->ci->_baseAddress + address_inc;
					asmjit_state->code->init(*asmjit_state->ci);
					asmjit_state->code->sync();
				}
			}
			else
			{
				asmjit_state->a->resetLastError();
				fprintf(stdout, "ERROR: 0x%08X: %s In line: %s\n", err, DebugUtils::errorAsString(err), &line[0]);
			}
		}
	}

	if(!start)
	{
		cout<<fg::yellow<<"Error : did not find a '; Start' mark \n";
		cout<<"Assuming the starting position right after" <<fg::reset<<endl;
		start = true;
		rewind(pFile);
		goto try_parse;
	}
}

static void interpret_file(const char* f, bool rawinfo, bool cmp_asm)
{
	static FILE* pFile;
	static bool are_bytes_found;

	if(arg_memblock == nullptr)
	{

		if (!(pFile = fopen(f,"rw+")))
		{
			printf("\nCan't open \"%s\".\n",f);
			printf( "Error code opening file: %d\n", errno );
			printf( "Error opening file: %s\n", strerror( errno ) );
			debug_pause();
		}

		const int line_size = 4096;
		char line[line_size];
		size_t len;
		int pch;
		are_bytes_found = false;
		dispatch = true;

		arg_memblock = &instr_memblock[instr_memblock_offset / sizeof(char*)];

		while(fgets(&line[0], line_size, pFile) != nullptr)
		{
			//the hook header must contain this info!
			if(baseArg == nullptr)
			{
				if(strncmp(&line[0],"--b",3)==0)
				{
					len = fstrlen(&line[0]);
					mark_mem(len * sizeof(char*));
					pch = getposition(&line[0], len, '=');
					baseArg = (&instr_memblock[instr_memblock_offset / sizeof(char*)] - arg_memblock)+len+arg_memblock;
					sprintf(baseArg, "%s", (&line[0]+pch+1));
				}
			}

			if(offsArg == nullptr)
			{
				if(strncmp(&line[0],"--o",3)==0)
				{
					len = fstrlen(&line[0]);
					mark_mem(len * sizeof(char*));
					pch = getposition(&line[0], len, '=');
					offsArg = (&instr_memblock[instr_memblock_offset / sizeof(char*)] - arg_memblock)+len+arg_memblock;
					sprintf(offsArg, "%s", (&line[0]+pch+1));
				}
			}

			if(archArg == nullptr)
			{
				if(strncmp(&line[0],"--a",3)==0)
				{
					len = fstrlen(&line[0]);
					pch = getposition(&line[0], len, '=');
					mark_mem(len * sizeof(char*));
					archArg = (&instr_memblock[instr_memblock_offset / sizeof(char*)] - arg_memblock)+len+arg_memblock;
					sprintf(archArg, "%s", (&line[0]+pch+1));
				}
			}

			char* pos = strstr(&line[0], "BYTES:");
			if(pos!=nullptr)
			{
				are_bytes_found = true;
				buffer_from_file_only = string(line);
				buffer_from_file_only = buffer_from_file_only.substr(pos-&line[0]+6);
				break;
			}
		}

		if(!hexToU64(baseAddress, baseArg, fstrlen(baseArg)))
		{
			cout<<"Invalid --baseVA parameter "<<baseArg<<endl;
			baseAddress = Globals::kNoBaseAddress;
		}
		if(!hexToU64(baseOffset, offsArg, fstrlen(offsArg)))
		{
			cout<<"Invalid --offs parameter "<<offsArg<<endl;
			baseOffset = 0;
		}

	}

	if(!are_bytes_found || cmp_asm)
	{
		cout<<fg::yellow;
		cout<<"Trying to compile assembly... In file  "<<f<<"\n";
		if(are_bytes_found)
		{
			cout<<"Warning! You are compiling a compiled file ! Trashing prev result. \n";
			buffer_from_file_only.clear();
		}
		else
		{
			cout<<"Note! Could not find a string of bytes . \n";
		}
		cout<<fg::reset;

		if(dispatch)
		{
			layer++;
			enter_asmjit_hook(0, nullptr, filename, ".comp");
			return;
		}
		else
		{
			rewind(pFile);
			compile_asm(pFile);
		}
	}

	fclose(pFile);
}

static void print_jit_asm_info(CodeInfo *ptr = nullptr)
{
	cout<<"===============================================================\n"
	    <<"Remote Assembler:\n"
	    <<"  - A simple command-line based instruction encoder\n";
	printf("  - Architecture=%s   [select by --arch=x86|x64]\n", archArg      );
	printf("  - Base-Address=%04X [select by --baseVA=hex]\n", baseAddress    );
	printf("  - Base-Offset=%04X  [select by --offs=hex]\n",baseOffset        );
	if(ptr!=nullptr)
	{
		printf("  - Current-Base-Address=%04X \n", ptr->_baseAddress          );
	}
	cout<<"---------------------------------------------------------------\n"
	    <<"Input:\n"
	    <<"  - Enter instruction and its operands to be encoded.\n"
	    <<"  - Available commands: \n"
	    <<"  - Enter '.info' to update the info. \n"
	    <<"  - Enter '.clear' to clear everything.\n"
	    <<"  - Enter '.print' to print the current code.\n"
	    <<"  - Enter '.org' to set base address, roffset and architecture.\n"
	    <<"  - Enter '.inc' value by which increment baseAddress Curr:"<<(address_inc)<<"\n"
	    <<"  - Enter '.save' to save the current code.\n"
	    <<"  - Enter '.undo' to revoke last encoded instruction.\n"
	    <<"  - Enter '.file' to change filename. Current: "+filename+"\n"
	    <<"  - Enter '.load' to load saved hook from file.\n"
	    <<"  - Enter '.write' to apply the given hook.\n"
	    <<"  - Enter '.wall' to apply all hooks. (files have to end with .jh) \n"
	    <<"  - Enter '.comp' to compile the current file \n"
	    <<"  - Enter '.up' to move one layer up in patcher memory. Curr: "<<layer<<"\n"
	    <<"  - Enter '.ret' return to main menu. \n"
	    <<"===============================================================\n";
}

static void write_all_jithooks(string path, string patchfile)
{
	boost::filesystem::path p(path);
	boost::filesystem::directory_iterator end_itr;
	string current_file;
	uint64_t baseOffset = 0;

	for (boost::filesystem::directory_iterator itr(p); itr != end_itr; ++itr)
	{
		if (boost::filesystem::is_regular_file(itr->path()))
		{
			current_file = itr->path().string();
			if (boost::filesystem::extension(current_file).compare(".jh")==0)
			{
				reset_globals();
				filename = current_file;
				interpret_file(&current_file[0], true, false);
				size_t len = fstrlen(offsArg);
				if(!hexToU64(baseOffset, offsArg, len))
				{
					cout<<fg::red<<"Failed to calculate baseOffset !"<<"In file: "<<current_file<<fg::reset<<endl;
					cout<<"Input: "<<offsArg<<"Len = "<<len<<endl;
					debug_pause();
				}
				FileIO file_out(patchfile, ios::out |ios::in |ios::binary);
				file_out.fWriteBinaryFile(HexToBytes(buffer_from_file_only),baseOffset,buffer_from_file_only.length()/2);
				printf("Written to: %s Offset=%s\n",&patchfile[0],offsArg);
			}
		}
	}
	return;
}

int enter_asmjit_hook(int argc, char* argv[], string patchfile, char* command)
{
	static bool Next = false;
	char* log;
	size_t len;
	size_t _size = 0;
	string tmp_filename;
	int step_bytes_length;
	char input[4095];

	if (archArg)
	{
		if (strncmp(archArg,"x86",3) == 0)
		{
			archType = ArchInfo::kTypeX86;
		}
		else if (strncmp(archArg,"x64",3) == 0)
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
		archArg = "x86";
	}

	if(argv!=nullptr)
	{
		CmdLine cmd(argc, argv);
		archArg = cmd.getKey("--arch");
		baseArg = cmd.getKey("--baseVA");
		offsArg = cmd.getKey("--offs");

		archType = ArchInfo::kTypeX64;
		baseAddress = Globals::kNoBaseAddress;

		size_t maxLen = archType == ArchInfo::kTypeX64 ? 16 : 8;
		if (baseArg)
		{
			len = fstrlen(baseArg);
			if (!len || len > maxLen || !hexToU64(baseAddress, baseArg, len))
			{
				cout<<"Invalid --baseVA parameter "<<baseArg<<endl;
				debug_pause();
			}
		}

		if (offsArg)
		{
			len = fstrlen(offsArg);
			if (!len || len > maxLen || !hexToU64(baseOffset, offsArg, len))
			{
				cout<<"Invalid --offs parameter "<<offsArg<<endl;
				debug_pause();
			}
		}
	}

	print_jit_asm_info();

	StringLogger logger;
	logger.addOptions(Logger::kOptionBinaryForm);

	CodeInfo ci(archType, 0, baseAddress);
	CodeHolder code;

	code.init(ci);
	code.setLogger(&logger);

	X86Assembler a(&code);
	AsmParser p(&a);
	CodeBuffer buffer;

	if(instr_memblock == nullptr)
	{
		instr_memblock = new char[MAX_MEMORY_SIZE];
	}

	asmjit_state = new ptr();
	asmjit_state->logger = &logger;
	asmjit_state->ci = &ci;
	asmjit_state->code = &code;
	asmjit_state->a = &a;
	asmjit_state->p = &p;
	asmjit_state->buffer = &buffer;

	for (;;)
	{
		// 0 is the section number, this case .text
		buffer = code.getSectionEntry(0)->getBuffer();
		_size = buffer.getLength();
		if(Next)
		{
			goto up;
		}

		if(command)
		{
			memcpy(input, command, fstrlen(command));
			dispatch = false;
			command = nullptr;
		}
		else
		{
			fgets(input, 4095, stdin);
			dispatch = true;
		}

		if (input[0] == 0) break;

		if (isCommand(input, ".comp"))
		{
			if(dispatch)
			{
				reset_globals();
			}
			interpret_file(&filename[0],true, true);
			continue; // stay on this layer.
		}

		if (isCommand(input, ".clear"))
		{
			code.reset(false);  // Detaches everything.
			code.init(ci);
			code.setLogger(&logger);
			code.attach(&a);
			ci._baseAddress = ci._baseAddress - (address_inc * encoded_instr.size());
			encoded_instr.clear();
			buffer_from_file_only.clear();
			continue;
		}

		if (isCommand(input, ".print"))
		{
			dumpCode(buffer,_size);
			continue;
		}

		if (isCommand(input, ".info"))
		{
			print_jit_asm_info(&ci);
			continue;
		}

		if (isCommand(input, ".inc"))
		{
			printf("Enter an integer. \n");
			fgets(input, 4095, stdin);
			address_inc = atoi(input);
			continue;
		}

		if (isCommand(input, ".org"))
		{
			printf("Enter baseAddress as hex ... example: `123` \n");
			fgets(input, 4095, stdin);
			if(!hexToU64(baseAddress, input, fstrlen(input)))
			{
				cout<<"Invalid --baseVA parameter "<<input<<endl;
				baseAddress = Globals::kNoBaseAddress;
			}
			printf("Enter baseOffset as hex ... example: `123` \n");
			fgets(input, 4095, stdin);
			if(!hexToU64(baseOffset, input, fstrlen(input)))
			{
				cout<<"Invalid --offs parameter "<<input<<endl;
				baseOffset = 0;
			}
			printf("Enter arch as x86|x64 \n");
			fgets(input, 4095, stdin);
			archArg = input;
			enter_asmjit_hook(0,nullptr,patchfile, ".clear");
			goto up;
		}

		if (isCommand(input, ".up"))
		{
up:
			code.reset(true);
			if(instr_memblock != nullptr && layer == 1)
			{
				reset_globals();
				delete instr_memblock;
				instr_memblock = nullptr;
				instr_memblock_offset = 0;
				Next = false;
				layer = 0;
				return 0;
			}
			if(layer != 0)
			{
				layer--;
			}
			return 0;
		}

		if (isCommand(input, ".ret"))
		{
			Next = true;
			return 0;
		}

		if (isCommand(input, ".write"))
		{
			if(!buffer_from_file_only.empty())
			{
				FileIO file_out(patchfile, ios::out |ios::in |ios::binary);
				file_out.fWriteBinaryFile(HexToBytes(buffer_from_file_only),baseOffset,buffer_from_file_only.length()/2);
				printf("Written to: %s \n",&patchfile[0]);
			}
			else
			{
				printf("Save the buffer, then load it, then write it. \n");
				printf("No buffer from file is loaded. \n");
			}
			continue;
		}

		if(isCommand(input, ".wall"))
		{
			printf("Folder where to loop ...\n");
			cin>>tmp_filename;
			write_all_jithooks(tmp_filename, patchfile);
			continue;
		}

		if (isCommand(input, ".save"))
		{
			if(buffer.hasData())
			{
				saveCode(buffer,&filename[0],baseAddress);
			}
			continue;
		}

		if (isCommand(input, ".file"))
		{
			printf("Set new filename...\n");
			cin>>filename;
			continue;
		}

		if (isCommand(input, ".load"))
		{
			printf("Filename to load ...\n");
			reset_globals();
			cin>>tmp_filename;
			interpret_file(&tmp_filename[0],true, false);
			printf("File loaded ...\n");
			enter_asmjit_hook(0,nullptr,patchfile);
			goto up;
		}

		if (isCommand(input, ".undo"))
		{
			if(!encoded_instr.empty())
			{
				encoded_instr.pop_back();
			}
			if(buffer.hasData())
			{
				//@@2 how many bytes to keep in buffer? 0 = complete wipe.
				size_t b = _size-((step_bytes_length-1)/2);
				code.resizeBuffer(&buffer,b,0);
			}
			continue;
		}

		logger.clearString();
		Error err = p.parse(input);

		if (err == kErrorOk)
		{
			log = logger.getString();
			len = logger.getLength();
			size_t i;

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
				step_bytes_length = len - i;
				char temp1[step_bytes_length];
				sprintf(&temp1[0], "%.*s", (int)(step_bytes_length), log + i);
				printf("%s", &temp1[0]);

				mark_mem(len * sizeof(char*));
				strncpy(instr_memblock+instr_memblock_offset, log, len+1);
				instr_memblock[(instr_memblock_offset / sizeof(char*))+len] = '\0';
				encoded_instr.push_back(instr_memblock+instr_memblock_offset);

				ci._baseAddress = ci._baseAddress + address_inc;
				code.init(ci);
				code.sync();
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
