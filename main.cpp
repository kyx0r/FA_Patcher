
#include "patcher/rang.hpp"

#include <iostream>
//#include <string>
#include <cstring>
#include <limits>
#include <time.h>
#include <stdio.h>
#include <vector>

#include <assert.h>
#include <fstream>

#include "patcher/binPatcher.hpp"

using namespace std;

//Note!!! Both are acceptable for FileIO class.
/* FileIO* file = new FileIO(target_in, ios::in | ios::binary | ios::ate); //object is on the heap
FileIO file(target_in, ios::in | ios::binary | ios::ate); //object is on the stack */

int main (void)
{	
	string reply;
	string target_in = "ForgedAlliance_base.exe";
	string target_out = "ForgedAlliance_exxt.exe";
	
	Patcher patch(target_in, target_out);
	BinSection section;
	Utils util;
	
	cout<<"What to do? p - full patch, s - patch ignoring hooks, cdbg - convert x64dbg disassembly to gcc inline \n";
	cout<<"h - only hooks. \n";
	cin >> reply;
	
	if(reply.compare("p")==0)
	{
		FileIO file_in(target_in, ios::in | ios::binary);		
		section.create_Section(file_in._file, target_out, ".exxt", 5242880,0x500000);
		FileIO file_out(target_out, ios::out |ios::in |ios::binary);
		section.apply_Ext(0xBDF000,file_out);
		function_table table = util.linker_map_parser("build/mapfile.map");
		util.write_def_table(table);
		Hooks hook(false, target_out);
		hook.parse_hooks();
	}
		
	if(reply.compare("h")==0)
	{
		Hooks hook(false, target_out);
		hook.parse_hooks();
	}
		
	boost::filesystem::copy_file("ForgedAlliance_exxt.exe", "C:/ProgramData/FAForever/bin/ForgedAlliance_exxt.exe",boost::filesystem::copy_option::overwrite_if_exists);	
	if(reply.compare("cdbg")==0)
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
	
	cout<<"Done."<<endl;	
	debug_pause();

	return 0;
}
