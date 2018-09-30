
#pragma once
#include "patcher.hpp"

class FileIO
{
	public:
	
	//FileIO(){};
	
	int get_file_size(const string& filename);
	char* fReadBinaryFile(const string& f);
	unsigned fWriteBinaryFile(const string& f,const char* HexValue, int offset, int Bytes_to_write);
	int get_bytes(const string& filename, bool errorRet = true);
	string rem_extension(string str);
	int get_line_count(fstream& file);
	
	private:
	
	streampos size;
	char *memblock;
	char ch;
	int count_bytes;
	int null_count;
	size_t pos;
};