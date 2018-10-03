
#pragma once
#include "patcher.hpp"
#include "fileIO.hpp"
#include "utils.hpp"

class Hooks : public Utils
{
	public:
	Hooks(bool hook_format = false, string filename = "");
	void parse_hooks();
	
	bool fast_Compile_Hooks = false; 
	bool indiv_Compile_Hooks = false;
	string filename_out;
	
	
	private:
	int compile_Hook(string current_file, string Final_Filename, string alone_Filename);
	void build_O(string current_file, string Final_Filename, string alone_Filename);
	void parse_build(int offset, string alone_Filename);
	void apply_Hook(string current_file, int offset);
	void align_hook(int align_sizeL ,string filename, string command);
	
};