
#pragma once
#include "patcher.hpp"
#include "fileIO.hpp"

struct function_table
{
	uint32_t section_alignment;
	vector <string> Name;
	vector <uint32_t> FunctionVirtualAddress;
};

class Utils : public FileIO
{
	public:
	int parse_offset(string filename, string expr = "0x");
	bool gpp_link(string filename, string command);
	function_table linker_map_parser(string filename);
	int write_def_table(function_table &table);
	
	template <typename ElemT>
	struct HexTo {
		ElemT value;
		operator ElemT() const {return value;}
		friend std::istream& operator>>(std::istream& in, HexTo& out) {
			in >> std::hex >> out.value;
			return in;
		}
	};
	
	private:
	
	string line;
	string word;
	int offset;
	size_t pos;
	char *tok;
	bool text_sec_found;
};