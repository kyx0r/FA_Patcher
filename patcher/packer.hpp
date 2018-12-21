#include "patcher.hpp"
#include "fileIO.hpp"
#include "utils.hpp"

using namespace binPatcher;

class FileIO;

class Packer : public Utils
{
	public:
	
	Packer();
	
	Packer(const string &execName);
	
	void packDataFile( const string &fileName);
	void unpackDataFile(const string &fileName);
	size_t readSize(FileIO &out);
	void writeSize();
	
	private:
	bool PACKERMAGIC = false;
	bool PACSTART = false;
	bool PACEND = false;
	string line;
	string outexec;
	FileIO orig;
	FileIO pac_file;
	size_t _size;
	size_t pos;

};