#include "jitHooks.hpp"

vector<char*> encoded_instr;
string buffer_from_file_only;
string filename = "./hooks/jithook.jh";
unsigned int address_inc = 3;
char* archArg;
char* baseArg;

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

void dumpCode(CodeBuffer buffer, size_t _size)
{
	printf("%s","Current hook buffer is:\n");
	for (size_t i = 0; i < _size; i++)
	{
		printf("%02X", buffer.getData()[i]);
	}
	printf(&buffer_from_file_only[0]);
}

void saveCode(CodeBuffer buffer, char* _filename, uint64_t baseAddress, char* archArg)
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
	fprintf(pFile, "--base=%04X \n", baseAddress);
	fprintf(pFile, "--arch=%s \n", archArg);
	for(size_t i = 0; i < _size; i++)
	{
		fwrite(encoded_instr[i], sizeof(char), ::strlen(encoded_instr[i]), pFile);
	}
	fwrite("BYTES:", sizeof(char), 6, pFile);
	fwrite(&buffer_from_file_only[0], sizeof(char), buffer_from_file_only.length(), pFile);
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

string load_file(const string&f)
{
	FileIO file(f);
	string line;
	string new_str;
	size_t pos;
	bool are_bytes_found = false;
	while(getline(file._file,line))
	{
		pos = line.find(":");
		if(pos!=string::npos)
		{
			are_bytes_found = true;
			new_str = line.substr(pos+1);
			break;
		}
	}
	if(new_str.empty() || !are_bytes_found)
	{
		cout<<"Error in function "<<__func__<<endl;
		cout<<"Could not find a string of bytes. \n";
		debug_pause();
	}
	return new_str;
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

void read_header(const char* f, bool rawinfo)
{
	FILE* pFile;
	if (!(pFile = fopen(f,"rw+")))
	{
		printf("\nCan't open \"%s\".\n",f);
		printf( "Error code opening file: %d\n", errno );
		printf( "Error opening file: %s\n", strerror( errno ) );
		debug_pause();
	}

	const size_t line_size = 300;
	char* line = malloc(line_size);
	size_t len;
	int pch;
	baseArg = new char[50];
	archArg = new char[50];
	while(fgets(line, line_size, pFile) != nullptr)
	{
		//the hook header must contain this info!
		if(strncmp(line,"--b",3)==0)
		{
			if(rawinfo)
			{
				len = strlen(line);
				pch = getposition(line, len, '=');
				sprintf(baseArg, "%s", (line+pch+1));
			}
			else if(!rawinfo)
			{
				len = strlen(line)-2;
				memcpy(baseArg, line, len);
			}
		}

		if(strncmp(line,"--a",3)==0)
		{
			if(rawinfo)
			{
				len = strlen(line);
				pch = getposition(line, len, '=');
				sprintf(archArg, "%s", (line+pch+1));
				break;
			}
			else if(!rawinfo)
			{
				len = strlen(line)-2;
				memcpy(archArg, line, len);
				break;
			}
		}
	}

	free(line);
	fclose(pFile);
}

void print_jit_asm_info()
{
	cout<<"===============================================================\n"
	    <<"Remote Assembler:\n"
	    <<"  - A simple command-line based instruction encoder\n";
	printf("  - Architecture=%s [select by --arch=x86|x64]\n", archArg        );
	printf("  - Base-Address=%s [select by --base=hex]\n", baseArg            );
	cout<<"---------------------------------------------------------------\n"
	    <<"Input:\n"
	    <<"  - Enter instruction and its operands to be encoded.\n"
	    <<"  - Available commands: \n"
	    <<"  - Enter '.info' to update the info. \n"
	    <<"  - Enter '.clear' to clear everything.\n"
	    <<"  - Enter '.print' to print the current code.\n"
	    <<"  - Enter '.org' to set base address and architecture.\n"
	    <<"  - Enter '.inc' value by which increment baseAddress Curr:"<<(address_inc)<<"\n"
	    <<"  - Enter '.save' to save the current code.\n"
	    <<"  - Enter '.undo' to revoke last encoded instruction.\n"
	    <<"  - Enter '.file' to change filename. Current: "+filename+"\n"
	    <<"  - Enter '.load' to load saved hook from file.\n"
	    <<"  - Enter '.write' to apply the given hook.\n"
	    <<"  - Enter '.wall' to apply all hooks. (files have to end with .jh) \n"
	    <<"  - Enter '.ret' return to main menu. \n"
	    <<"===============================================================\n";
}

void write_all_jithooks(string path, string patchfile)
{
	boost::filesystem::path p(path);
	boost::filesystem::directory_iterator end_itr;
	string current_file;
	uint64_t baseAddress = Globals::kNoBaseAddress;

	for (boost::filesystem::directory_iterator itr(p); itr != end_itr; ++itr)
	{
		if (boost::filesystem::is_regular_file(itr->path()))
		{
			current_file = itr->path().string();
			if (boost::filesystem::extension(current_file).compare(".jh")==0)
			{
				buffer_from_file_only = load_file(current_file);
				read_header(&current_file[0], true);
				if(!buffer_from_file_only.empty())
				{
					size_t len = strlen(baseArg)-2;
					char* new_base = (char *)_alloca(len);
					memcpy(new_base, baseArg, len);
					if(!hexToU64(baseAddress, new_base, len))
					{
						cout<<fg::red<<"Failed to calculate baseAddress !"<<"In file: "<<current_file<<fg::reset<<endl;
						cout<<"Input: "<<new_base<<"Len = "<<len<<endl;
						debug_pause();
					}
					FileIO file_out(patchfile, ios::out |ios::in |ios::binary);
					file_out.fWriteString(buffer_from_file_only,baseAddress);
					printf("Written to: %s Offset=%s\n",&patchfile[0],baseArg);
				}
				else
				{
					printf("No buffer from file is loaded. \n");
					break;
				}
			}
		}
	}
	return;
}

int enter_asmjit_hook(int argc, char* argv[], string patchfile)
{
	CmdLine cmd(argc, argv);
	archArg = cmd.getKey("--arch");
	baseArg = cmd.getKey("--base");

	uint32_t archType = ArchInfo::kTypeX64;
	uint64_t baseAddress = Globals::kNoBaseAddress;
	char* log;
	char* temp;
	size_t len;
	size_t _size = 0;
	string tmp_filename;

	/* 	cout<<"Cmd args are: \n";
		cout<<"Counter = "<<argc<<endl;
		for(int i = 0; i < argc; ++i)
			cout << argv[i] <<i<< '\n'; */

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
		archArg = "x86";
	}

	if (baseArg)
	{
		size_t len = ::strlen(baseArg);
		size_t maxLen = archType == ArchInfo::kTypeX64 ? 16 : 8;

		if (!len || len > maxLen || !hexToU64(baseAddress, baseArg, len))
		{
			printf("Invalid --base parameter\n");
			debug_pause();
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

	char input[4096];
	for (;;)
	{
		// 0 is the section number, this case .text
		buffer = code.getSectionEntry(0)->getBuffer();
		_size = buffer.getLength();

		fgets(input, 4095, stdin);
		if (input[0] == 0) break;

		if (isCommand(input, ".clear"))
		{
			code.reset(false);  // Detaches everything.
			code.init(ci);
			code.setLogger(&logger);
			code.attach(&a);
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
			print_jit_asm_info();
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
			printf("Enter as --base=hex ... example: `--base=123` \n");
			fflush(stdin);
			fgets(input, 4095, stdin);

			//FIX the cmd parser expects a non null terminated string
			//This is a hack around that.
			size_t len = strlen(input)-1;
			char* new_base_address = (char *)malloc(len);
			memcpy(new_base_address, input, len);

			printf("Enter as --arch=x86|x64 \n");
			fflush(stdin);
			fgets(input, 4095, stdin);
			len = strlen(input)-1;
			char* new_arch = (char *)malloc(len);
			memcpy(new_arch, input, len);

			char* head_info[2] = {new_arch,new_base_address};
			enter_asmjit_hook(2,head_info,patchfile);
		}

		if (isCommand(input, ".ret"))
		{
			code.reset(true);
			buffer_from_file_only.clear(); //this does not clean up completely, yet.
			return 0;
		}

		if (isCommand(input, ".write"))
		{
			if(!buffer_from_file_only.empty())
			{
				FileIO file_out(patchfile, ios::out |ios::in |ios::binary);
				file_out.fWriteString(buffer_from_file_only,baseAddress);
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
				saveCode(buffer,&filename[0],baseAddress,archArg);
			}
			continue;
		}

		if (isCommand(input, ".file"))
		{
			printf("Set new filename...\n");
			fflush(stdin);
			cin>>filename;
			continue;
		}

		if (isCommand(input, ".load"))
		{
			printf("Filename to load ...\n");
			fflush(stdin);
			cin>>tmp_filename;
			buffer_from_file_only = load_file(tmp_filename);
			read_header(&tmp_filename[0]);
			printf("File loaded ...\n");
			char* head_info[2] = {archArg,baseArg};
			enter_asmjit_hook(2,head_info,patchfile);
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
				code.resizeBuffer(&buffer,0,0);
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
				temp = new char[len];
				strncpy(temp,log,len);
				encoded_instr.push_back(temp);
				printf("%.*s", (int)(len - i), log + i);
				ci._baseAddress = baseAddress + address_inc;
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
	delete temp;
	return 0;
}