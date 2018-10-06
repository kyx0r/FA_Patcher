
#include "FileIO.hpp"

FileIO::FileIO(string filename, ios_base::openmode mode)
: _file(filename, mode)
{
	if(!filename.empty())
	{
		if(!_file)
		{
			cout <<fg::red<< "Cannot open "<<filename<<endl;
			cin.get();
			exit(1);
		}	
	}
}

FileIO::~FileIO()
{
	_file.close();
}

void FileIO::errexit(string filename)
{
	cout<<fg::red<<"Unable to open file "<<filename<<endl;
	cout<<fg::red<<"Usage: string filename or call class constructor instead, and this function with no input filename!"<<endl;
	cin.get();
	exit(1);	
}

int FileIO::get_file_size(string filename) 
{
	if(!filename.empty())
	{
		ifstream _file(filename, ios::binary | ios::ate);
		if(_file.is_open())
		{
			return _file.tellg();
		}
		else
		{
			cout<<fg::red<<"Incorrect arguments in function "<<__func__<<endl;
			errexit(filename);
		}
	}
	if(_file.is_open())
	{
		return _file.tellg();
	}
	else
	{
		cout<<fg::red<<"Incorrect arguments in function "<<__func__<<endl;
		errexit(filename);
	}
}

vector<char> FileIO::fReadBinaryFile(string filename)
{
	if(!filename.empty())
	{
		ifstream _file(filename, ios::in|ios::binary|ios::ate);
		if(_file.is_open())
		{
			size = _file.tellg();
			vector<char> memblock(size);
			_file.seekg(0, ios::beg);
			_file.read(memblock.data(), size);
			_file.close();
			return memblock;
		}
		else
		{
			cout<<fg::red<<"Incorrect arguments in function "<<__func__<<endl;
			errexit(filename);
		}
	}
	if(_file.is_open())
	{
		size = _file.tellg();
		vector<char> memblock(size);
		_file.seekg(0, ios::beg);
		_file.read(memblock.data(), size);
		return memblock;	
	}
	else
	{
		cout<<fg::red<<"Incorrect arguments in function "<<__func__<<endl;
		errexit(filename);
	}
}

template <class T>
void clean_vector(vector<T> vect)
{
	vect = vector<T>();
}

unsigned FileIO::fWriteBinaryFile(vector<char> HexValue, int offset, int Bytes_to_write, string filename)
{
	if(!filename.empty())
	{
		fstream _file(filename, fstream::out |fstream::in |fstream::binary);
		if(_file.is_open())
		{
			_file.seekg(fstream::beg+offset);
			_file.write(HexValue.data(), Bytes_to_write);
			_file.close();
			clean_vector(HexValue);	
			return 1;
		}
		else
		{
			cout<<fg::red<<"Incorrect arguments in function "<<__func__<<endl;
			errexit(filename);			
		}
	}
	if(_file.is_open())
	{
		_file.seekg(fstream::beg+offset);
		_file.write(HexValue.data(), Bytes_to_write);
		clean_vector(HexValue);
		return 1;
	}
	else
	{
		cout<<fg::red<<"Incorrect arguments in function "<<__func__<<endl;
		errexit(filename);	
	}
}

int FileIO::parse_binary_end(fstream& _file, const string& filename, bool errorRet, int count_bytes, int null_count)
{
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

int FileIO::get_bytes(string filename, bool errorRet)
{
	if(!filename.empty())
	{
		fstream _file(filename, ios::in | ios::binary);
		if(_file.is_open())
		{
			return parse_binary_end(_file, filename, errorRet);
		}
		else
		{
			cout<<fg::red<<"Incorrect arguments in function "<<__func__<<endl;
			errexit(filename);				
		}
	}
	if(_file.is_open())
	{
		return parse_binary_end(_file, filename, errorRet);
	}
	else
	{
		cout<<fg::red<<"Incorrect arguments in function "<<__func__<<endl;
		errexit(filename);	
	}
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

int FileIO::get_line_count(fstream& file)
{
	file.unsetf(ios_base::skipws);
	return count(istream_iterator<char>(file),istream_iterator<char>(),'\n');
}