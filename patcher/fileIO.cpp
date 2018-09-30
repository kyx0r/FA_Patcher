
#include "FileIO.hpp"

int FileIO::get_file_size(const string &filename) 
{
	ifstream file( filename.c_str(), ios::binary | ios::ate);
	return file.tellg();
}

char* FileIO::fReadBinaryFile(const string&f)
{
	ifstream input (f.c_str(), ios::in|ios::binary|ios::ate);
	if (input.is_open())
	{
		size = input.tellg();
		memblock = new char [size];
		input.seekg (0, ios::beg);
		input.read (memblock, size);
		input.close();
	}
	else
	{		
		cout <<fg::red<< "Unable to open file"<<f<<endl;
		cin.get();
		exit(1);
	}
	return memblock;	
}

unsigned FileIO::fWriteBinaryFile(const string& f,const char* HexValue, int offset, int Bytes_to_write)
{
	fstream output (f.c_str(), fstream::out |fstream::in |fstream::binary);
	if(output.fail() || !output)
	{
		cerr <<fg::red<< "Error opening the file: " << strerror(errno);
		cin.get();
		exit(1);
	}
	output.seekg(fstream::beg+offset);
	output.write(HexValue, Bytes_to_write);
	output.close();
	return 1;
}

int FileIO::get_bytes(const string& filename, bool errorRet)
{
	count_bytes = 0;
	null_count = 0;
	ifstream in(filename, ios::in | ios::binary);
	if(!in)
	{
		cout <<fg::red<< "Cannot open " << filename <<endl;
		cin.get();
		exit(1);
	}
	do
	{
		if(in.get(ch))
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
	in.close();
	return count_bytes;
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