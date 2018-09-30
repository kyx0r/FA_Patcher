
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

int main (void)
{	
	
	string target_in = "ForgedAlliance_base.exe";
	string target_out = "ForgedAlliance_exxt.exe";

	Patcher patch(target_in);
	BinSection section;
	Utils util;
	Hooks hook(false, target_out);
	
	ifstream pe_file(target_in, ios::in | ios::binary);
	if(!pe_file)
	{
		cout <<fg::red<< "Cannot open " <<endl;
		cin.get();
		exit(1);
		return false;
	}
	
	section.create_Section(pe_file, target_out, ".exxt", 5242880,0x500000);
	section.apply_Ext(0xBDF000);
	function_table table = util.linker_map_parser("build/mapfile.map");
	util.write_def_table(table);
	hook.parse_hooks();
	
	boost::filesystem::copy_file("ForgedAlliance_exxt.exe", "C:/ProgramData/FAForever/bin/ForgedAlliance_exxt.exe",boost::filesystem::copy_option::overwrite_if_exists);	
	
	cout<<"Done."<<endl;
	
	cin.get();

	return 0;
}
