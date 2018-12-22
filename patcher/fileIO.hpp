
#pragma once
#include "patcher.hpp"

class FileIO
{
public:
	fstream _file;
	string filename;

	explicit FileIO(string filename = "", ios_base::openmode mode = ios::in);
	~FileIO();

	int get_file_size();
	vector<char> fReadBinaryFile(int offset = 0);
	unsigned fWriteBinaryFile(vector<char> HexValue, int offset, int Bytes_to_write);
	unsigned fWriteString(const string &HexValue, int offset);
	int get_bytes(bool errorRet = true);
	string rem_extension(string str);
	int get_line_count();
	int parse_binary_end(bool errorRet, int count_bytes = 0, int null_count = 0);

private:
	streampos size;
	char ch;
	size_t pos;
};