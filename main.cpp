

#include <iostream>
//#include <string>
#include <cstring>
#include <limits>
#include <time.h>
#include <stdio.h>
#include <vector>

#include <assert.h>
#include <fstream>
//#include <filesystem>

#include <boost/thread/thread.hpp>
#include <boost/filesystem.hpp>
#include <boost/tokenizer.hpp>
#include <boost/lexical_cast.hpp>

using namespace std;

int get_file_size(const string &filename) 
{
	ifstream file( filename.c_str(), ios::binary | ios::ate);
	return file.tellg();
}

inline char*fReadBinaryFile(const string&f)
{
	streampos size;
	char * memblock;
	ifstream input (f.c_str(), ios::in|ios::binary|ios::ate);
	if (input.is_open())
	{
		size = input.tellg();
		memblock = new char [size];
		input.seekg (0, ios::beg);
		input.read (memblock, size);
		input.close();
	}
	else
	{		
		cout << "Unable to open file";
		cin.get();
		exit(1);
	}
	return memblock;	
}

inline unsigned fWriteBinaryFile(const string& f,const char* HexValue, int offset, int Bytes_to_write)
{
	fstream output (f.c_str(), fstream::out |fstream::in |fstream::binary);
	if(output.fail() || !output)
	{
		cerr << "Error opening the file: " << strerror(errno);
		return -1;
	}
	output.seekg(fstream::beg+offset);
	output.write(HexValue, Bytes_to_write);
	output.close();
	return 1;
}

int get_bytes(string name)
{
	FILE *p_file = nullptr;
	p_file = fopen(name.c_str(),"rb");
	int ch = EOF;
	int count_bytes=0;
	int null_count=0;
	do
	{
		ch = fgetc(p_file);
		if (EOF == ch)
		{ 
			if (ferror(p_file))
			{
				perror("fgetc() failed");
			}
			else
			{
				cout<<"Unexpected end of a file"<<name<<endl;
			}
			cin.get();
			exit(EXIT_FAILURE);
		}
		count_bytes++;
		if(ch==0)
		{
			count_bytes--;
			null_count++;
		}
		else
		{
			count_bytes+=null_count;
			null_count=0;
		}
		//cout<<count_bytes<<endl;
		//cout<<null_count<<endl;
	} 
	while (count_bytes-null_count>0); 
	//we need to determine the end of assembly, but nullcheck is not perfect
	//so we check for a sequence of nulls to make sure its truly the end. Because some assembly opcodes have nulls in them. 
	fclose(p_file);
	return count_bytes;
}

bool check_system()
{
	if (system(nullptr))
	{
		cout << "Command processor exists \n";
		cout << " " "\n";
		return true;
	}
	else
	{
		cout << "Command processor doesn't exist \n";
		return false;
	}
}  

//this awesomeness taken from here:
//https://stackoverflow.com/questions/1070497/c-convert-hex-string-to-signed-integer
template <typename ElemT>
struct HexTo {
    ElemT value;
    operator ElemT() const {return value;}
    friend std::istream& operator>>(std::istream& in, HexTo& out) {
        in >> std::hex >> out.value;
        return in;
    }
};

bool gpp_link(string filename)
{
	string command = "make gpp_link PRIME_NAME=";
	command.append(filename);
	string filename_tmp = filename.append(".tmp");
	string tmp_Prefix = " TMP_NAME=";
	tmp_Prefix.append(filename_tmp);
	command.append(tmp_Prefix);
	
	cout<<"GENERATING .TMP FILES ------------------------------->"<<endl;
	//here convert the binary to MS PE format.
	if(system(&command[0]))
	{
		printf("Link error. \"%s\"\n");
		return false;
	}
	cout<<"\n";
	
	command = "make rip_out_binary";
	command.append(tmp_Prefix);
	string filename_prime = " PRIME_NAME=";
	command.append(filename_prime);
	filename_prime = filename.append(".bin");
	command.append(filename_prime);
	
	cout<<"GENERATING .BIN FILES --------------------------->"<<endl;
	
	if(system(&command[0]))
	{
		printf("Ripping binary  error. \"%s\"\n");
		return false;
	}
	cout<<"\n";
	return true;
}

void Apply_Hook(string alone_Filename, int offset)
{
	char *hook_F = fReadBinaryFile(alone_Filename);
	int Bytes_to_write = get_bytes(alone_Filename);
	cout<<"APPLY HOOK : "<<alone_Filename <<"    Number of instructions: "<<Bytes_to_write<<endl;
	fWriteBinaryFile("ForgedAlliance_exxt.exe", hook_F, offset, Bytes_to_write);
	cout<<"\n";
}

string rem_extension(string str)
{
	size_t pos = str.find(".");
	if (pos!=string::npos)
	{
		string ext = str.substr(pos);
		str.erase( str.size() - ext.length());
		return str;
	}
	else
	{
		cout<<"No extension in passed string"<<str<<endl;
		return str;
	}
}

void Parse_build(int offset, string alone_Filename)
{
	boost::filesystem::path p("./build");
	boost::filesystem::directory_iterator end_itr;
	alone_Filename = rem_extension(alone_Filename);
	for (boost::filesystem::directory_iterator itr(p); itr != end_itr; ++itr)
    {
		if (boost::filesystem::is_regular_file(itr->path())) 
		{
            string current_file = itr->path().string();
			if(boost::filesystem::extension(current_file).compare(".o")==0)
			{
				if(current_file.find(alone_Filename)!=string::npos)
				{
					if(!gpp_link(current_file))
					{
						exit(1);
					}
				}	
			}
			if(boost::filesystem::extension(current_file).compare(".bin")==0)
			{
				if(current_file.find(alone_Filename)!=string::npos)
				{
					Apply_Hook(current_file, offset);
					break;
				}
			}
		}
	}
}

void build_O(string dir_hook, string hook_name, string alone_Filename)
{
	cout<<"BUILDING .O FILES --------------------------------->";
	string command = "make _hooks OBJ_NAME_=";
	command.append(hook_name);
	alone_Filename.insert(0, " OBJS=");
	command.append(alone_Filename);
	cout<<"\n";
	system(&command[0]);
	cout<<"\n";	
}

int Compile_Hook(string dir_hook, string hook_name, string alone_Filename)
{
	fstream hook(dir_hook);
	string line;
	if(hook.is_open())
	{
		while(getline(hook,line))
		{
			if(line.find("ROffset = ")!=string::npos)
			{
				char* tok = strtok(&line[0], "0x");
				line.erase(0,strlen(tok)+1);
				line.insert(0,"0");
				cout<<"ROffset = "<<line<<endl;
				int offset = boost::lexical_cast<HexTo<int>>(line);
				build_O(dir_hook, hook_name, alone_Filename);
				Parse_build(offset, alone_Filename);
				break;
			}
			else
			{
				cout<<"Could not find ROffset in the hook : "<<dir_hook<<endl;
				cin.get();
				exit(1);
			}
		}
		hook.close();
	}
	else
	{
		cout<<"Unable to open the hook file : "<<dir_hook<<endl;
		cin.get();
		exit(1);
	}
	return 1;
}

void Parse_hooks()
{
	boost::filesystem::path p("\hooks");
	boost::filesystem::directory_iterator end_itr;
	cout<<""<<endl;
	cout<<"Available hooks : \n";
	for (boost::filesystem::directory_iterator itr(p); itr != end_itr; ++itr)
    {
		if (boost::filesystem::is_regular_file(itr->path())) 
		{
            string current_file = itr->path().string();
			size_t pos = current_file.find("hook_");
			if (pos!=string::npos)
			{
				string end = current_file.substr (pos);
				string Final_Filename;
				Final_Filename.append(end);
				string alone_Filename = Final_Filename;
				cout<<Final_Filename<<endl;
				Final_Filename.append(".o");
				Final_Filename.insert(0,"../build/");
				Compile_Hook(current_file,Final_Filename,alone_Filename);
			}
        }
	}	
}

bool gpp_Compile()
{	
	if(!check_system())
	{
		printf("\No system present. Exiting patcher. \"%s\".\n");
		cin.get();
		exit(1);
	}
	
	system("make directories");
	
	const int verisign_offset = 0xBDD000;
	char verisign_size = 0x1500;
	cout<<"Patching the verisign code \n";
	fWriteBinaryFile("ForgedAlliance_exxt.exe", &verisign_size, verisign_offset, 8);
	
	system("make ext_sector");
	gpp_link("build/ext_sector.o");
	Parse_hooks();
	
	char *ext_F = fReadBinaryFile("build/ext_sector.o.tmp.bin");
	
	cout<<"APPLY .EXT SECTION \n";
	fWriteBinaryFile("ForgedAlliance_exxt.exe", ext_F, verisign_offset, get_file_size("build/ext_sector.o.tmp.bin"));
}

bool init_Ext()
{
	char PE_header_values[10][15] = {"\x07\x00\x1F\x49\x4A\x4A\x00\x00","\x00\xC5\xE8\x00\x00\x10\x00\x00",
	"\xAC\x24\xBE\x00\x02\x00\x00\x00","\xF0\xE3\x00\xEC\xB8\x04\x00",
	"\x00\x00\x00\x00\x00\x00\x00\x00","\x2E\x65\x78\x74\x00\x00\x00\x00",
	"\x00\x15\x00\x00\x00\xB0\xE8\x00","\x00\x15\x00\x00\x00\xD0\xBD\x00",
	"\x00\x00\x00\x00\x00\x00\x00\x00","\x00\x00\x00\x00\x20\x00\x00\x60"};
	
	
	const int PE_header_address [] = {0x136, 0x180, 0x188, 0x1B9, 0x1C8, 0x318, 0x320, 0x328, 0x330, 0x338};
	

	for(int i=0; i<=sizeof(PE_header_address)/sizeof(PE_header_address[0]); i++)
	{	
		fWriteBinaryFile("ForgedAlliance_exxt.exe", PE_header_values[i], PE_header_address[i], 8);
	}
	
	return true;
}

int main (void)
{
	if (!boost::filesystem::exists("ForgedAlliance_base.exe"))
	{
		cout<<"ForgedAlliance_base.exe not found! Rename the file if needed\n";
		cin.get();
		exit(1);
	}
		
	boost::filesystem::copy_file("ForgedAlliance_base.exe", "ForgedAlliance_exxt.exe",boost::filesystem::copy_option::overwrite_if_exists);
	
	init_Ext();
	
	gpp_Compile();
	
	cin.get(); 

	return 0;
	
}
