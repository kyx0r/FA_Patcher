
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
//#include <asmjit_lib/asmjit.h>
//#include <asmjit_lib/x86.h>
#include "patcher/binPatcher.hpp"

using namespace std;

//Note!!! Both are acceptable for FileIO class.
/* FileIO* file = new FileIO(target_in, ios::in | ios::binary | ios::ate); //object is on the heap
FileIO file(target_in, ios::in | ios::binary | ios::ate); //object is on the stack */

typedef int (*Func)(void);
using namespace asmjit;

int main (int argc, char* argv[])
{

	enter_asmjit_hook(argc,argv);


	/*    JitRuntime rt;                          // Runtime specialized for JIT code execution.

	  CodeHolder code;                        // Holds code and relocation information.
	  code.init(rt.getCodeInfo());            // Initialize to the same arch as JIT runtime.

	  X86Assembler a(&code);                  // Create and attach X86Assembler to `code`.
	  a.mov(x86::eax, 1);                     // Move one to 'eax' register.
	  a.ret();

	  code.sync();

	  CodeBuffer& buf = code.getSectionEntry(0)->getBuffer();

	  for (size_t i = 0; i < buf.getLength(); i++)
	  printf("%02X", buf.getData()[i]);

	  ofstream outfile ("test.txt");

	  outfile<<&code<<endl; */


	// Return from function.
	// ----> X86Assembler is no longer needed from here and can be destroyed <----

	//Func fn;
// Error err = rt.add(&fn, &code);         // Add the generated code to the runtime.
// if (err) return 1;                      // Handle a possible error returned by AsmJit.
	// ----> CodeHolder is no longer needed from here and can be destroyed <----

// int result = fn();                      // Execute the generated code.
// printf("%d\n", result);                 // Print the resulting "1".

	// All classes use RAII, all resources will be released before `main()` returns,
	// the generated function can be, however, released explicitly if you intend to
	// reuse or keep the runtime alive, which you should in a production-ready code.
//  rt.release(fn);


	/* 	string reply;
		string target_in = "ForgedAlliance_base.exe";
		string target_out = "ForgedAlliance_exxt.exe";

		Patcher patch(target_in, target_out);
		binPatcher::BinSection section;
		binPatcher::Utils util;

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
			binPatcher::Hooks hook(false, target_out);
			hook.parse_hooks();
		}

		if(reply.compare("h")==0)
		{
			binPatcher::Hooks hook(false, target_out);
			hook.parse_hooks();
		}

		//tests only.
		//boost::filesystem::copy_file("ForgedAlliance_exxt.exe", "C:/ProgramData/FAForever/bin/ForgedAlliance_exxt.exe",boost::filesystem::copy_option::overwrite_if_exists);

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

		cout<<"Done."<<endl;	 */
	debug_pause();

	return 0;
}
