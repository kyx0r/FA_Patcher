
#include "fileIO.hpp"

FileIO::FileIO(string filename, ios_base::openmode mode)
: _file(filename, mode)
{
	if(!filename.empty())
	{
		this->filename = filename;
		if(!_file)
		{
			cout <<fg::red<< "Cannot open "<<filename<<endl;
			cout <<fg::red<<"Usage: 1st arg: string filename, 2nd arg: optional ios flag, ie ios::in."<<endl;
			cin.get();
			exit(1);
		}	
	}
}

FileIO::~FileIO()
{
	_file.close();
}

int FileIO::get_file_size() 
{
	return _file.tellg();
}

vector<char> FileIO::fReadBinaryFile()
{
	size = _file.tellg();
	vector<char> memblock(size);
	_file.seekg(0, ios::beg);
	_file.read(memblock.data(), size);
	return memblock;	
}

unsigned FileIO::fWriteBinaryFile(vector<char> HexValue, int offset, int Bytes_to_write)
{
	_file.seekg(offset);
	_file.write(HexValue.data(), Bytes_to_write);
	HexValue = vector<char>();
	return 1;	
}

int FileIO::parse_binary_end(bool errorRet, int count_bytes, int null_count)
{
	_file.seekg(ios::beg);
	do
	{
		if(_file.get(ch))
		{
			count_bytes++;
			if(ch==0)
			{
				count_bytes--;
				null_count++;
			}
			else
			{
				count_bytes+=null_count;
				null_count=0;
			}
		}
		else
		{
			cout<<fg::yellow<<"Unexpected end of a file"<<filename<<fg::reset<<endl;
			if(errorRet)
			{	
				return false;
			}
			else if(!errorRet)
			{
				return count_bytes;
			}			
		}
	}
	while(count_bytes-null_count>0);
	return count_bytes;	
}

int FileIO::get_bytes(bool errorRet)
{
	return parse_binary_end(errorRet);	
}

string FileIO::rem_extension(string str)
{
	pos = str.find_last_of(".");
	if (pos!=string::npos)
	{
		string ext = str.substr(pos);
		str.erase( str.size() - ext.length());
		return str;
	}
	else
	{
		cout<<fg::yellow<<"No extension in passed string"<<str<<fg::reset<<endl;
		return str;
	}
}

int FileIO::get_line_count()
{
	_file.unsetf(ios_base::skipws);
	return count(istream_iterator<char>(_file),istream_iterator<char>(),'\n');
}