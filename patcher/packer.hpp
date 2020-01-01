#include "patcher.hpp"
#include "fileIO.hpp"
#include "utils.hpp"

using namespace binPatcher;

class FileIO;

class Packer : public Utils
{
public:

	Packer();

	~Packer();

	Packer(const string &execName);

	void packDataFile( const string &fileName, string dir = "");
	void unpackDataFiles(string fileName = "");
	void packDirectory(const string &pth);
	size_t readSize();
	void writeSize();
	void cleanUp(const string &file);

private:
	bool PACKERMAGIC = false;
	bool PACSTART = false;
	bool PACEND = false;
	bool EXPLICIT_FILE = false;
	string line;
	string outexec;
	FileIO orig;
	FileIO pac_file;
	ofstream outfile;
	size_t _size;
	size_t pos;
	size_t ret = 0;

};