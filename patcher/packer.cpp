#include "packer.hpp"


#ifdef defined(WIN32) || defined(_WIN32) || defined(_WIN64)
extern char binary_data_txt_start;
extern const void* binary_data_txt_size;
const size_t len = (size_t)&binary_data_txt_size;
#else
extern char _binary_data_txt_start;
extern const void* _binary_data_txt_size;
const size_t len = (size_t)&_binary_data_txt_size;
#endif

const char* getPackDataFile()
{

}

void packDataFiles( const char* fileName, string execName )
{
	cout<<"packing...\n";
	
	boost::filesystem::copy_file(execName, "FaPatcherPac.exe", boost::filesystem::copy_option::overwrite_if_exists);
	
	FileIO file("FaPatcherPac.exe", ios::out | ios::in | ios::binary);
	FileIO orig(execName);
	
	file.fWriteString("\nPACKERMAGIC", orig.get_file_size());
	
	FileIO out(fileName);
	
	string line;
	while(getline(out._file,line))
	{	
		file.fWriteString("\n", file.get_file_size());
		file.fWriteString(line, file.get_file_size());
		//file.fWriteString("\n", file.get_file_size());
	}
	
}