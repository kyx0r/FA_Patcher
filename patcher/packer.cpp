#include "packer.hpp"

Packer::Packer()
:orig(uname)
{
	outexec = orig.rem_extension(uname);
	outexec+="pac.exe";
	
	if (!boost::filesystem::exists(outexec))
	{
		boost::filesystem::copy_file(uname, outexec, boost::filesystem::copy_option::overwrite_if_exists);
	}
	new(&pac_file) FileIO(outexec, ios::out | ios::in | ios::binary);
}

Packer::Packer(const string &execName)
:orig(execName)
{
	outexec = uname;
	
	if (!boost::filesystem::exists(outexec))
	{
		boost::filesystem::copy_file(execName, outexec, boost::filesystem::copy_option::overwrite_if_exists);
	}
	new(&pac_file) FileIO(outexec, ios::out | ios::in | ios::binary);
}

void Packer::unpackDataFile(const string &fileName)
{
	FileIO out(outexec);
	_size = readSize(out);
	out._file.seekg(_size);
	
	while(getline(out._file,line))
	{	
		//cout<<line<<endl;
		if(line.compare("PACKERMAGIC")==0)
		{
			cout<<fg::green<<"Packer entry found... "<<fg::reset<<endl;
			PACKERMAGIC = true;
		}
		if(line.find("PACSTART-")!=string::npos)
		{
			if(inc_search(line,fileName,9)!=string::npos)
			{
				cout<<fg::green<<"Found entry: "<<line<<fg::reset<<endl;
				PACSTART = true;
			}
		}
		if(PACSTART)
		{
			if(line.find("PACEND-")==string::npos)
			{
				ofstream outfile(fileName);
				outfile<<line;
			}
			else
			{
				cout<<fg::green<<"Found end: "<<line<<fg::reset<<endl;
				PACSTART = false;
				break;
			}
		}
	}
	
	if(!PACKERMAGIC)
	{
		cout<<fg::red<<"Failed to find the data!... "<<fg::reset<<endl;
	}
}

void Packer::packDataFile( const string &fileName)
{
	cout<<"packing...\n";
	
	_size = orig.get_file_size();
	pac_file.fWriteString("\nPACKERMAGIC", _size);
	pac_file.fWriteString("\nPACSTART-"+fileName, pac_file.get_file_size());
	
	FileIO out(fileName);
	
	while(getline(out._file,line))
	{	
		pac_file.fWriteString("\n", pac_file.get_file_size());
		pac_file.fWriteString(line, pac_file.get_file_size());
	}
	pac_file.fWriteString("\nPACEND-"+fileName, pac_file.get_file_size());
	
	cout<<fg::green<<"File "<<fileName<<" packed!"<<fg::reset<<endl;
}

size_t Packer::readSize(FileIO &out)
{
	_size = out.get_file_size();
	cout<<_size<<endl;
	out._file.seekg(_size-50);	
	while(getline(out._file,line))
	{
		cout<<line<<endl;
	}
	return 0;
	//return boost::lexical_cast<size_t>(line);
}

void Packer::writeSize()
{
	_size = orig.get_file_size();
	pac_file.fWriteString("\n"+to_string(_size),_size);	
}