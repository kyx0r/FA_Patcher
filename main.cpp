
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

	cout<<fg::cyan
	    <<"Basic functions: \n"
	    <<"1 - use jithook. \n"
	    <<"2 - convert x64dbg disassembly to gcc inline. \n"
	    <<"Note: Features below require g++ and make present. \n"
	    <<"3 - full patch. \n"
	    <<"4 - patch ignoring hooks. \n"
	    <<"5 - only hooks. \n"
	    <<fg::reset<<endl;

	cin >> reply;
	if(reply.at(0)=='1')
	{
		enter_asmjit_hook(argc,argv,target_out);
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

		if(reply.at(0)=='3')
		{
			FileIO file_in(target_in, ios::in | ios::binary);
			section.create_Section(file_in._file, target_out, ".exxt", 5242880,0x500000);
			FileIO file_out(target_out, ios::out |ios::in |ios::binary);
			section.apply_Ext(0xBDF000,file_out);
			function_table table = util.linker_map_parser("build/mapfile.map");
			util.write_def_table(table);
			binPatcher::Hooks hook(false, target_out);
			hook.parse_hooks();
		}

		if(reply.at(0)=='4')
		{
			FileIO file_in(target_in, ios::in | ios::binary);
			section.create_Section(file_in._file, target_out, ".exxt", 5242880,0x500000);
			FileIO file_out(target_out, ios::out |ios::in |ios::binary);
			section.apply_Ext(0xBDF000,file_out);
			function_table table = util.linker_map_parser("build/mapfile.map");
			util.write_def_table(table);
		}

		if(reply.at(0)=='5')
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
	debug_pause();

	return 0;
}
