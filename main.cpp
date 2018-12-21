
#include "patcher/rang.hpp"
#include <iostream>
#include "patcher/binPatcher.hpp"

using namespace std;

//Note!!! Both are acceptable for FileIO class.
/* FileIO* file = new FileIO(target_in, ios::in | ios::binary | ios::ate); //object is on the heap
FileIO file(target_in, ios::in | ios::binary | ios::ate); //object is on the stack */

int main (int argc, char* argv[])
{

	string reply;
	string target_in = "ForgedAlliance_base.exe";
	string target_out = "ForgedAlliance_exxt.exe";

	Patcher patch(target_in, target_out);

ret:

	cout<<fg::cyan
	    <<"Basic functions: \n"
	    <<"1 - use jithook. \n"
	    <<"2 - convert x64dbg disassembly or masm assembly style to gcc inline. \n"
		<<"3 - packfile... \n"
		<<"4 - packdir... \n"
		<<"5 - unpack files ... \n"
	    <<"Note: Features below require g++ and make present. \n"
	    <<"6 - full patch. \n"
	    <<"7 - patch ignoring hooks. \n"
	    <<"8 - only hooks. \n"
	    <<fg::reset<<endl;

	cin >> reply;
	if(reply.at(0)=='1')
	{
		enter_asmjit_hook(argc,argv,target_out);
		goto ret;
	}
	
	if(reply.at(0)=='3')
	{
		cout<<"Enter Path to file... \n";
		Packer pack;
		cin >> reply;
		pack.packDataFile(reply);
		pack.writeSize();
		goto ret;
	}
	
	if(reply.at(0)=='4')
	{
		//packDataFiles("C_lib.txt","./FaPatcher.exe");
	}
	
	if(reply.at(0)=='5')
	{
		cout<<"Enter file to search... \n";
		Packer unpack("FaPatcherpac.exe");
		cin >> reply;
		unpack.unpackDataFile(reply);
		goto ret;		
	}

	binPatcher::BinSection section;
	binPatcher::Utils util;

	if(reply.at(0)=='2')
	{
		reply.empty();
		cout<<"Enter filename: ( 'd' for default. [preprocessor/x64dbg.asm] ) \n";
		cin>>reply;
		if(reply.compare("d")==0)
		{
			reply="preprocessor/x64dbg.asm";
		}
		x64dbg_parser_struct parser_struct = util.x64dbg_to_gcc_inline(reply);
		util.write_gcc_asm(reply, parser_struct);
	}

	if(patcher_error!=true)
	{

		if(reply.at(0)=='6')
		{
			FileIO file_in(target_in, ios::in | ios::binary);
			section.create_Section(file_in._file, target_out, ".exxt", 5242880,0x500000);
			FileIO file_out(target_out, ios::out |ios::in |ios::binary);
#ifdef OS_WIN
			section.apply_Ext(0xBDF000,file_out);
			function_table table = util.linker_map_parser("build/mapfile.map");
			util.write_def_table(table);
#else
			cout<<fg::yellow<<"Loading a c++ section is not suppored due to different binary format. "
			    <<"It still needs a new alignment parser, and a custom linker file probably. "
			    "You can still use the allocated section memory for something."<<fg::reset<<endl;
#endif
			binPatcher::Hooks hook(false, target_out);
			hook.parse_hooks();
		}

		if(reply.at(0)=='7')
		{
			FileIO file_in(target_in, ios::in | ios::binary);
			section.create_Section(file_in._file, target_out, ".exxt", 5242880,0x500000);
			FileIO file_out(target_out, ios::out |ios::in |ios::binary);
			section.apply_Ext(0xBDF000,file_out);
			function_table table = util.linker_map_parser("build/mapfile.map");
			util.write_def_table(table);
		}

		if(reply.at(0)=='8')
		{
			binPatcher::Hooks hook(false, target_out);
			hook.parse_hooks();
		}

	}
	else
	{
		cout<<"One or more errors follow. \n";
	}

	//tests only.
	//boost::filesystem::copy_file("ForgedAlliance_exxt.exe", "C:/ProgramData/FAForever/bin/ForgedAlliance_exxt.exe",boost::filesystem::copy_option::overwrite_if_exists);

	cout<<"Done."<<endl;
	goto ret;

	return 0;
}
