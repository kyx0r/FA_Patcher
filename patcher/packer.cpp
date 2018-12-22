#include "packer.hpp"

Packer::Packer()
	:orig(uname)
{
	outexec = orig.rem_extension(uname);
	outexec+="pac.exe";
	boost::filesystem::copy_file(uname, outexec, boost::filesystem::copy_option::overwrite_if_exists);
	new(&pac_file) FileIO(outexec, ios::out | ios::in | ios::binary);
}

Packer::Packer(const string &execName)
	:orig(execName)
{
	outexec = uname;
	boost::filesystem::copy_file(execName, outexec, boost::filesystem::copy_option::overwrite_if_exists);
	new(&pac_file) FileIO(outexec, ios::out | ios::in | ios::binary);
}

void Packer::unpackDataFiles(string fileName)
{
	_size = readSize();
	orig._file.seekg(_size+1);
	//+1 because stream will encounter binary data with wrong encoding and just silently terminate.
	if(!fileName.empty())
	{
		new(&outfile) ofstream(fileName);
		EXPLICIT_FILE = true;
	}

	while(getline(orig._file,line))
	{
		//cout<<line<<endl;
		if(line.compare("PACKERMAGIC")==0)
		{
			cout<<fg::green<<"Packer entry found... "<<fg::reset<<endl;
			PACKERMAGIC = true;
		}

		if(line.find("PACDIR-")!=string::npos)
		{
			string pth = line;
			pth.erase(0,7);
			boost::filesystem::path dir(pth);
			if(!boost::filesystem::exists(dir))
			{
				if (boost::filesystem::create_directory(dir))
				{
					cout<<fg::green<<"Directory created: "<<pth<<fg::reset<<endl;
				}
				else
				{
					cout<<fg::yellow<<"Failed to create directory: "<<pth<<fg::reset<<endl;
					cout<<fg::yellow<<"Check permissions!"<<fg::reset<<endl;
				}
			}
		}

		if(line.find("PACSTART-")!=string::npos)
		{
			if(!EXPLICIT_FILE)
			{
				fileName = line;
				fileName.erase(0,9);
			}
			if(inc_search(line,fileName,9)!=string::npos)
			{
				cout<<fg::green<<"Found entry: "<<line<<fg::reset<<endl;
				new(&outfile) ofstream(fileName);
				PACSTART = true;
				continue;
			}
		}
		if(PACSTART)
		{
			if(line.find("PACEND-")==string::npos)
			{
				outfile<<line<<endl;
			}
			else
			{
				cout<<fg::green<<"Found end: "<<line<<fg::reset<<endl;
				PACSTART = false;
				continue;
			}
		}
	}

	if(!PACKERMAGIC)
	{
		cout<<fg::red<<"Failed to find the data!... "<<fg::reset<<endl;
	}
}

void Packer::packDataFile( const string &fileName, string dir)
{
	cout<<"packing...\n";

	pac_file.fWriteString("\nPACKERMAGIC", orig.get_file_size());
	if(!dir.empty())
	{
		pac_file.fWriteString("\nPACDIR-"+dir, pac_file.get_file_size());
	}
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

void Packer::packDirectory(const string &pth)
{
	boost::filesystem::path p(pth);
	boost::filesystem::directory_iterator end_itr;

	for (boost::filesystem::directory_iterator itr(p); itr != end_itr; ++itr)
	{
		if (boost::filesystem::is_regular_file(itr->path()))
		{
			string current_file = itr->path().string();
			packDataFile(current_file,pth);
			pth.clear();
		}
	}
}

size_t Packer::readSize()
{
	_size = pac_file.get_file_size();
	size_t save_size = _size;
	size_t ret = 0;
	unsigned int offset = 50;
start:
	_size = _size - offset;
	//-50 should be pretty good range where we start from end
	//we only assume to find something here.
	pac_file._file.seekg(_size);
	while(getline(pac_file._file,line))
	{
		try
		{
			ret = boost::lexical_cast<size_t>(line);
		}
		catch(const bad_cast &bc)
		{
			_size++;
			pac_file._file.seekg(_size);
			continue;
		}
	}
	if(ret == 0)
	{
		cout<<fg::yellow<<"Note: no size found at "<<offset<<" offset"<<endl;
		cout<<"Trying to increase search range... "<<" offset"<<fg::reset<<endl;
		offset=offset+(save_size/10);
		if(offset >= save_size)
		{
			cout<<fg::red<<"Error: terminating due to missing size value."<<endl;
			debug_pause();
		}
		goto start;
	}
	return ret;
}

void Packer::writeSize()
{
	_size = orig.get_file_size();
	pac_file.fWriteString("\n"+to_string(_size),pac_file.get_file_size());
}