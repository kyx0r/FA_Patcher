
#pragma once
#include "patcher.hpp"

class FileIO
{
	public:
	fstream _file;
	
	explicit FileIO(string filename = "", ios_base::openmode mode = ios::in);
	~FileIO();
	
	int get_file_size(string filename = "");
	vector<char> fReadBinaryFile(string filename = "");
	unsigned fWriteBinaryFile(vector<char> HexValue, int offset, int Bytes_to_write, string filename = "");
	int get_bytes(string filename = "", bool errorRet = true);
	string rem_extension(string str);
	int get_line_count(fstream& file);
	
	private:
	void errexit(string filename);
	int parse_binary_end(fstream& _file, const string& filename, bool errorRet, int count_bytes = 0, int null_count = 0);
	
	streampos size;
	char ch;
	size_t pos;
};