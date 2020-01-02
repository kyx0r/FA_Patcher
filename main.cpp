
#include "patcher/rang.hpp"
#include <iostream>
#include "patcher/binPatcher.hpp"
#include "./tinycc/libtcc.h"

using namespace std;

//Note!!! Both are acceptable for FileIO class.
/* FileIO* file = new FileIO(target_in, ios::in | ios::binary | ios::ate); //object is on the heap
FileIO file(target_in, ios::in | ios::binary | ios::ate); //object is on the stack */

int main (int argc, char* argv[])
{

	string reply;
	ParseSettings("settings.ini");
	int len;
	Patcher _patch(target_in, target_out);
	setjmp(jump_buffer);

ret:

	cout<<fg::cyan
	    <<"Basic functions: \n"
	    <<"0 - choose file to patch \n"
	    <<"1 - use jithook. \n"
	    <<"2 - convert x64dbg disassembly or masm assembly style to gcc inline. \n"
	    <<"3 - packfile... \n"
	    <<"4 - packdir... \n"
	    <<"5 - unpack files ... \n"
	    <<"Note: Features below require g++ and make present. \n"
	    <<"6 - full patch. \n"
	    <<"7 - patch ignoring hooks. \n"
	    <<"8 - only hooks. \n"
	    <<"9 - remove a byte pattern from binary file. (in dev, do not use if unknown) \n"
	    <<"a - test/compile a file using tcc \n"
		<<"b - remove last section from PE file \n"
	    <<fg::reset<<endl;

	cin >> reply;

	if(reply.at(0)=='0')
	{
		cout<<"Enter input name : \n";
		cin>>target_in;
		cout<<"Enter output name : \n";
		cin>>target_out;
		Patcher patch(target_in, target_out);
		goto ret;
	}

	if(reply.at(0)=='1')
	{
		enter_asmjit_hook(argc,argv,target_out);
		goto ret;
	}

	if(reply.at(0)=='3')
	{
		cout<<"How many files to pack? \n";
		Packer pack;
		cin>>len;
		for (int z = 0; z < len; z++)
		{
			cout<<"Enter Path to file... Ex: ./main.cpp \n";
			cin >> reply;
			pack.packDataFile(reply);
		}
		pack.writeSize();
		goto ret;
	}

	if(reply.at(0)=='4')
	{
		cout<<"How many dirs to pack? \n";
		Packer pack;
		cin >> len;
		for (int z = 0; z < len; z++)
		{
			cout<<"Enter Path to... Ex: ./hooks \n";
			cin >> reply;
			pack.packDirectory(reply);
		}
		pack.writeSize();
		goto ret;
	}

	if(reply.at(0)=='5')
	{
		string copy = uname;
		FileIO dummy;
		copy = dummy.rem_extension(copy);
		copy+="pac.exe";

		Packer unpack(copy);
		unpack.unpackDataFiles();
		unpack.cleanUp(uname);
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
		int alignment;
		cout<<"Enter number by which to alighn jmp and call instructions [*ext = 0x0]\n";
		cin>>alignment;
		x64dbg_parser_struct parser_struct = util.x64dbg_to_gcc_inline(reply, alignment);
		util.write_gcc_asm(reply, parser_struct);
	}

	if(reply.at(0)=='a')
	{
		TCCState* stcc = tcc_new();
		tcc_set_options(stcc, &tccargs[0]);
		tcc_add_file(stcc, &tccinput[0]);
		tcc_output_file(stcc, &tccoutput[0]);
	}

	if(patcher_error!=true)
	{

		if(reply.at(0)=='6')
		{
			FileIO file_in(target_in, ios::in | ios::binary);
			section.create_Section(file_in._file, target_out, sectionName, sectionSize, sectionVSize);
			FileIO file_out(target_out, ios::out |ios::in |ios::binary);
#ifdef OS_WIN
			section.apply_Ext(sectionRoffset, file_out);
			function_table table = util.linker_map_parser(linkmapfile, 0x1000);
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
			section.create_Section(file_in._file, target_out, sectionName, sectionSize, sectionVSize);
			FileIO file_out(target_out, ios::out |ios::in |ios::binary);
			section.apply_Ext(sectionRoffset, file_out);
			function_table table = util.linker_map_parser(linkmapfile, 0x1000);
			util.write_def_table(table);
		}

		if(reply.at(0)=='8')
		{
			binPatcher::Hooks hook(false, target_out);
			hook.parse_hooks();
		}

		if(reply.at(0)=='9')
		{
			reply.empty();
			cout<<"Enter byte pattern to remove \n";
			cin>>reply;
			util.FindAndRemoveBytePattern(target_in, {"64890D00000000", "64a100000000", "64a300000000", "64892500000000"}, "CCCCCCC3");
		}
		
		if(reply.at(0)=='b')
		{
			FileIO file_in(target_in, ios::in | ios::binary);
			section.remove_last_Section(file_in._file, target_out);
		}
		
	}
	else
	{
		cout<<"One or more errors follow. (if stuck restart the pacther) \n";
	}

	//tests only.
	//boost::filesystem::copy_file("ForgedAlliance_exxt.exe", "C:/ProgramData/FAForever/bin/ForgedAlliance_exxt.exe",boost::filesystem::copy_option::overwrite_if_exists);

	if(strcmp(&copytodir[0], "null") != 0)
	{
		boost::filesystem::copy_file(target_out, copytodir, boost::filesystem::copy_option::overwrite_if_exists);
	}

	if(delbuild)
	{
		boost::filesystem::remove_all(buildDir);
		boost::filesystem::create_directory(buildDir);
	}

	cout<<"Done."<<endl;
	goto ret;

	return 0;
}
