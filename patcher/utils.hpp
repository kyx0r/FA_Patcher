
#pragma once
#include "patcher.hpp"
#include "fileIO.hpp"

struct function_table
{
	uint32_t section_alignment;
	vector <string> Name;
	vector <uint32_t> FunctionVirtualAddress;
};

struct x64dbg_parser_struct
{
	vector <string> GccInstruction = {"__asm__", "("};
};

namespace binPatcher
{

class Utils : public FileIO
{
public:
	int parse_offset(FileIO& file, string expr = "0x");
	bool gpp_link(string filename, string command);
	function_table linker_map_parser(string filename);
	void write_def_table(function_table &table);
	void write_gcc_asm(string dbg_inline_file, x64dbg_parser_struct &table);
	x64dbg_parser_struct x64dbg_to_gcc_inline(string dbg_inline_file, int align_calls=0x1000);
	string cut_on_first_null(string line);
	string add_quotations(string line);
	size_t inc_search(string target, string searched, size_t pos = 0);

	template <typename ElemT>
	struct HexTo
	{
		ElemT value;
		operator ElemT() const
		{
			return value;
		}
		friend std::istream& operator>>(std::istream& in, HexTo& out)
		{
			in >> std::hex >> out.value;
			return in;
		}
	};

private:

	string line;
	string word;
	string _word;
	int offset;
	size_t pos;
	size_t tmp_pos;
	char *tok;
	bool text_sec_found;
};

} //namespace binPatcher