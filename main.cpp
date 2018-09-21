

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

#include "pe_lib/pe_bliss.h"

static bool fast_Compile_Hooks = false; //rely on internal makefile system
static bool indiv_Compile_Hooks = true;

using namespace std;
using namespace pe_bliss;

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
		cin.get();
		exit(1);
	}
	output.seekg(fstream::beg+offset);
	output.write(HexValue, Bytes_to_write);
	output.close();
	return 1;
}

int get_bytes(string filename, bool errorRet = true)
{
	char ch;
	int count_bytes=0;
	int null_count=0;
	ifstream in(filename, ios::in | ios::binary);
	if(!in)
	{
		cout << "Cannot open " << filename <<endl;
		cin.get();
		exit(1);
	}
	do
	{
		if(in.get(ch))
		{
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
		}
		else
		{
			cout<<"Unexpected end of a file"<<filename<<endl;
			if(errorRet)
			{	
				return false;
			}
			else if(!errorRet)
			{
				return count_bytes;
			}			
		}
	}
	while(count_bytes-null_count>0);
	in.close();
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

string rem_extension(string str)
{
	size_t pos = str.find_last_of(".");
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

bool gpp_link(string filename, string command)
{
	command.append(" PRIME_NAME=");
	command.append(filename);
	filename = rem_extension(filename);
	string filename_tmp = filename.append(".tmp");
	string tmp_Prefix = " TMP_NAME=";
	tmp_Prefix.append(filename_tmp);
	command.append(tmp_Prefix);
	
	cout<<"GENERATING .TMP FILES ------------------------------->"<<endl;
	//here convert the binary to MS PE format.
	if(system(&command[0]))
	{
		printf("Link error. \"%s\"\n");
		cin.get();
		exit(1);
	}
	cout<<"\n";
	
	command = "make rip_out_binary";
	command.append(tmp_Prefix);
	string filename_prime = " PRIME_NAME=";
	command.append(filename_prime);
	filename = rem_extension(filename);
	filename_prime = filename.append(".bin");
	command.append(filename_prime);
	
	cout<<"GENERATING .BIN FILES --------------------------->"<<endl;
	
	if(system(&command[0]))
	{
		printf("Ripping binary  error. \"%s\"\n");
		cin.get();
		exit(1);
	}
	cout<<"\n";
	return true;
}

void align_hook(int align_sizeL ,string filename, string command)
{
	filename = rem_extension(filename);
	filename.append(".o");
	gpp_link(filename, "make hook_gpp_link align_size=" + to_string(align_sizeL));
}

void apply_Hook(string current_file, int offset)
{
	int align_sizeL;
	char *hook_F = fReadBinaryFile(current_file);
	int Bytes_to_write = get_bytes(current_file);
	while(Bytes_to_write == false) //in case the hook is bigger then supposable allocate more memory. 
	{
		align_sizeL = get_bytes(current_file, Bytes_to_write) * 2;
		align_hook(align_sizeL, current_file, "make hook_gpp_link PRIME_NAME=");
		Bytes_to_write = get_bytes(current_file);	
	}
	cout<<"APPLY HOOK : "<<current_file <<"    Number of instructions: "<<Bytes_to_write<<endl;
	fWriteBinaryFile("ForgedAlliance_exxt.exe", hook_F, offset, Bytes_to_write);
	cout<<"\n";
}

void parse_build(int offset, string alone_Filename)
{
	boost::filesystem::path p("./build");
	boost::filesystem::directory_iterator end_itr;
	alone_Filename = rem_extension(alone_Filename);
	string current_file;
	for (boost::filesystem::directory_iterator itr(p); itr != end_itr; ++itr)
    {
		if (boost::filesystem::is_regular_file(itr->path())) 
		{
            current_file = itr->path().string();
			if(boost::filesystem::extension(current_file).compare(".o")==0)
			{
				if(current_file.find(alone_Filename)!=string::npos)
				{
					if(!gpp_link(current_file, "make hook_gpp_link"))
					{
						cout<<"gpp_link error"<<endl;
						cin.get();
						exit(1);
					}
					break;
				}	
			}
		}
	}
	for (boost::filesystem::directory_iterator itr(p); itr != end_itr; ++itr)
    {
		if (boost::filesystem::is_regular_file(itr->path())) 
		{
            current_file = itr->path().string();
			if(boost::filesystem::extension(current_file).compare(".bin")==0)
			{
				if(current_file.find(alone_Filename)!=string::npos)
				{
					apply_Hook(current_file, offset);
					break;
				}
			}
		}
	}
}

void build_O(string current_file, string Final_Filename, string alone_Filename)
{
	cout<<"BUILDING .O FILES --------------------------------->";
	string command = "make _hooks OBJ_NAME_=";
	command.append(Final_Filename);
	alone_Filename.insert(0, " OBJS=");
	command.append(alone_Filename);
	cout<<"\n";
	system(&command[0]);
	cout<<"\n";	
}

int parse_offset(string filename, string expr = "0x")
{
	fstream file(filename);
	string line;
	int offset;
	char *tok;
	size_t pos;
	if(file.is_open())
	{
		while(getline(file,line))
		{
			pos = line.find(expr);
			if(pos!=string::npos)
			{
				line = line.substr(pos);
				tok = strtok(&line[0], "0x");
				line.erase(0,strlen(tok)+1);
				line.insert(0,"0");
				offset = boost::lexical_cast<HexTo<int>>(line);
				file.close();
				return offset;
			}
		}
		cout<<"Could not find : "<<expr<<" in the file : "<<filename<<endl;
		cin.get();
		exit(1);
	}
	else
	{
		cout<<"Unable to open the file : "<<filename<<endl;
		cin.get();
		exit(1);
	}
	return 1;	
}

int compile_Hook(string current_file, string Final_Filename, string alone_Filename)
{
	int offset = parse_offset(current_file, "ROffset = ");
	cout<<"ROffset = "<<hex<<offset<<endl;
	if(fast_Compile_Hooks)
	{
		system("make _fast_hooks");
		fast_Compile_Hooks = false;
	}
	else if (indiv_Compile_Hooks)
	{	
		build_O(current_file, Final_Filename, alone_Filename);
	}
	parse_build(offset, alone_Filename);
	return 1;
}

void parse_hooks()
{
	boost::filesystem::path p("\hooks");
	boost::filesystem::directory_iterator end_itr;
	cout<<"\n";
	cout<<"Available hooks : \n";
	cout<<"\n";
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
				Final_Filename = rem_extension(Final_Filename);
				Final_Filename.append(".o");
				Final_Filename.insert(0,"../build/");
				compile_Hook(current_file,Final_Filename,alone_Filename);
			}
        }
	}	
}

struct image_section_header
{
	vector <string> Name;
	vector <uint32_t> VirtualSize;
	vector <uint32_t> VirtualAddress;
	vector <uint32_t> SizeOfRawData;
};

image_section_header populate_image_section_header(const string &filename)
{
	image_section_header header;
	ifstream pe_file(filename, ios::in | ios::binary);
	if(!pe_file)
	{
		cout << "Cannot open " << filename <<endl;
		cin.get();
		exit(1);
	}	
	
	try
	{
		pe_base image(pe_factory::create_pe(pe_file));
		//cout << "Reading PE sections..." << hex << showbase << endl << endl;
		const section_list sections(image.get_image_sections());
		for(section_list::const_iterator it = sections.begin(); it != sections.end(); ++it)
		{
			const section& s = *it; 
/* 			std::cout << "Section [" << s.get_name() << "]" << std::endl 
				<< "Size of raw data: " << s.get_size_of_raw_data() << std::endl 
				<< "Virtual address: " << s.get_virtual_address() << std::endl 
				<< "Virtual size: " << s.get_virtual_size() << std::endl 
				<< std::endl; */
			header.Name.push_back(s.get_name());
			header.VirtualSize.push_back(s.get_virtual_size());
			header.VirtualAddress.push_back(s.get_virtual_address()); 
			header.SizeOfRawData.push_back(s.get_size_of_raw_data()); 
		}
	}
	catch(const pe_exception& e)
	{
		std::cout << "Error: " << e.what() << std::endl;
		cin.get();
		exit(1);
	}
	
	return header;
}

struct function_table
{
	uint32_t section_alignment;
	vector <string> Name;
	vector <uint32_t> FunctionVirtualAddress;
};

int get_line_count(fstream& file)
{
	file.unsetf(ios_base::skipws);
	return count(istream_iterator<char>(file),istream_iterator<char>(),'\n');
}

int write_def_table(function_table &table)
{
	ofstream outfile ("preprocessor/define.h");
	
	for(int i=0; i<table.Name.size(); i++)
	{
		outfile<<"#define "<<table.Name[i]<<" 0x"<<hex<<table.FunctionVirtualAddress[i]<<endl;
	}
	outfile.close();
}

function_table linker_map_parser(string filename)
{
	function_table table;
	string line;
	string word;
	int offset;
	int pos;
	
	ifstream map_file(filename);
	bool text_sec_found = false;
	if(!map_file)
	{
		cout << "Cannot open map_file : " << filename <<endl;
		cin.get();
		exit(1);
	}
	table.section_alignment = parse_offset(filename, "__section_alignment__ = "); 	
	while(getline(map_file,line))
	{
		if(line.find("*(.text.*)")!=string::npos)
		{break;}
		if(line.find("0x")==string::npos)
		{continue;}
		if(line.find(".text")!=string::npos)
		{text_sec_found = true; continue;}
	
		if(text_sec_found == true)
		{
			if(line.find("../build/")==string::npos)
			{
				line.resize(line.find("("));
				pos = line.find("::");
				if(pos!=string::npos)
				{
					line.replace(pos,2,"__");
				}
				stringstream ss(line);
				while(ss>>word)
				{
					if(word.find("0x")!=string::npos)
					{
						offset = boost::lexical_cast<HexTo<int>>(word);
						table.FunctionVirtualAddress.push_back(offset-table.section_alignment);
					}
					else
					{
						table.Name.push_back(word);
					}
				}
			}
		}
	}	
	map_file.close();
	return table;
}

void apply_Ext()
{
	//	const int verisign_offset = 0xBDD000; //.ext
	const int verisign_offset = 0xBDF000; //.exxt
	uint32_t align_data = 0;
	uint32_t align_rdata = 0;
	uint32_t align_bss = 0;
	uint32_t align_idata = 0;
	string make_ext_gpp_link = "make ext_gpp_link";
	system("make ext_sector");
	
	image_section_header header = populate_image_section_header("build/exxt_sector.o");
	
	//There might be a better way of doing this, but for now all I got. 
	//This should be very realiable still, cause locations will be the same as original binary. 
	for(int i=0; i<header.Name.size(); i++)
	{
		if(header.Name[i].compare(".data") == 0)
		{
			align_data = header.VirtualAddress[i];
			make_ext_gpp_link.append(" align_data=" + to_string(align_data));
		}
		if(header.Name[i].compare(".rdata") == 0)
		{
			align_rdata = header.VirtualAddress[i]-align_data;
			make_ext_gpp_link.append(" align_rdata=" + to_string(align_rdata));
		}
		if(header.Name[i].compare(".bss") == 0)
		{
			align_bss = header.VirtualAddress[i]-align_rdata;
			make_ext_gpp_link.append(" align_bss=" + to_string(align_bss));
		}
		if(header.Name[i].compare(".idata") == 0)
		{
			align_idata = header.VirtualAddress[i]-align_bss;
			make_ext_gpp_link.append(" align_idata=" + to_string(align_idata));
		}
	}

	gpp_link("build/exxt_sector.o", make_ext_gpp_link);
	char *ext_F = fReadBinaryFile("build/exxt_sector.bin");
	
	cout<<"APPLY .EXT SECTION   Number of instructions: "<<get_file_size("build/exxt_sector.bin")<<endl;
	fWriteBinaryFile("ForgedAlliance_exxt.exe", ext_F, verisign_offset, get_file_size("build/exxt_sector.bin"));
	
	cout<<"Parsing linker map file..."<<endl;
	function_table table = linker_map_parser("build/mapfile.map");
	write_def_table(table);
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
	apply_Ext();
	parse_hooks();
}

bool create_Section(istream& pe_file, const string& name, int raw_size = 1, int virtual_size = 0x1000)
{
	pe_base image(pe_factory::create_pe(pe_file));
		
	section new_section;
	new_section.readable(true).writeable(true).executable(true).code(true);
	new_section.set_name(name); 
	new_section.resize_raw_data(raw_size);
	//new_section.set_size_of_raw_data(raw_size);
	//cout<<new_section.get_size_of_raw_data()<<endl;
		
	section& added_section = image.add_section(new_section);
	image.set_section_virtual_size(added_section, virtual_size);
		
	string out_file_name = "ForgedAlliance_exxt.exe";
	string::size_type slash_pos;
	if((slash_pos = out_file_name.find_last_of("/\\")) != string::npos)
	{
		out_file_name = out_file_name.substr(slash_pos + 1);
	}
		
	ofstream new_pe_file(out_file_name.c_str(), ios::out | ios::binary | ios::trunc);
	if(!new_pe_file)
	{
		cout << "Cannot create " << out_file_name <<endl;
		cin.get();
		exit(1);
		return false;
	}
	rebuild_pe(image, new_pe_file);
		
	cout << "PE was rebuilt and saved to " << out_file_name <<endl; 	
	
	return true;
}

bool init_Ext(string filename)
{
	ifstream pe_file(filename, ios::in | ios::binary);
	if(!pe_file)
	{
		cout << "Cannot open " << filename <<endl;
		cin.get();
		exit(1);
		return false;
	}
	
	try
	{
		create_Section(pe_file, ".exxt", 5242880,0x500000);
	}
	catch(const pe_exception& e)
	{
		cout << "Error: " << e.what() <<endl;
		cin.get();
		exit(1);
		return false;
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
	
	if(!init_Ext("ForgedAlliance_base.exe"))
	{
		cin.get();
		exit(1);
	}

 	gpp_Compile(); 
	
	boost::filesystem::copy_file("ForgedAlliance_exxt.exe", "C:/ProgramData/FAForever/bin/ForgedAlliance_exxt.exe",boost::filesystem::copy_option::overwrite_if_exists);
	
	cout<<"Done."<<endl;
	
	//cin.get(); 

	return 0;
	
}
