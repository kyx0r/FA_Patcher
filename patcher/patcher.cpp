
#include "patcher.hpp"
#include "fileIO.hpp"
#include "utils.hpp"

string make;
string uname;
jmp_buf jump_buffer;
bool patcher_error;

string target_in;
string target_out;
string sectionName;
string linkmapfile;
string copytodir;
string buildDir;
int sectionRoffset;
int sectionSize;
int sectionVSize;
int delbuild;

void ParseSettings(string filename)
{
	if(!boost::filesystem::exists(filename))
	{
		std::ofstream f(filename);
		cout<<fg::yellow<<filename<<" not found, creating default... \n"<<fg::reset;
		f << "target_in=ForgedAlliance_base.exe" <<endl;
		f << "target_out=ForgedAlliance_exxt.exe" <<endl;
		f << "linkmapfile=build/mapfile.map"<<endl;
		f << "buildDir=./build"<<endl;
		f << "copytodir=null" <<endl;
		f << "sectionName=.exxt"<<endl;
		f << "sectionRoffset=0xBDF000"<<endl;
		f << "sectionSize=0x80000"<<endl;
		f << "sectionVSize=0x50000"<<endl;
		f << "delbuild=0x0" <<endl;	
	}
	//not the most optimal performance, but hey, at least position in file does not matter.
	FileIO file(filename);
	binPatcher::Utils util;
	sectionRoffset = util.parse_offset(file, "sectionRoffset=");
	sectionSize = util.parse_offset(file, "sectionSize=");
	sectionVSize = util.parse_offset(file, "sectionVSize=");
	delbuild = util.parse_offset(file, "delbuild=");
	target_in = util.parse_string(file, "target_in=");
	target_out = util.parse_string(file, "target_out=");
	sectionName = util.parse_string(file, "sectionName=");
	linkmapfile = util.parse_string(file, "linkmapfile=");
	buildDir = util.parse_string(file, "buildDir=");
	copytodir = util.parse_string(file, "copytodir=");
	cout<<fg::green<<"Configuration : \n";
	file._file.clear();
    file._file.seekg(0,std::ios::beg);
	string line;
	while(getline(file._file,line))
	{
		cout<<line<<"\n";
	}
	cout<<"------------------------------\n"<<fg::reset;
}

Patcher::Patcher(string filename_in, string filename_out)
{
	if (!boost::filesystem::exists(filename_in))
	{
		cout<<fg::red<<filename_in<<" not found! Rename the file if needed\n"<<fg::reset;
		patcher_error = 1;
	}
	else
	{
		boost::filesystem::copy_file(filename_in, filename_out, boost::filesystem::copy_option::overwrite_if_exists);
	}

	if(!boost::filesystem::exists(buildDir))
	{
		boost::filesystem::create_directory(buildDir);
	}

	if(!check_system())
	{
		cout<<fg::red<<"No system present. Abandoning the ship. \n";
		patcher_error = 1;
	}

	if(!check_make())
	{
		cout<<fg::red<<"No make present. Exiting patcher. \n";
		patcher_error = 1;
	}

	if(!check_gcc())
	{
		cout<<fg::red<<"No g++/gcc present. Exiting patcher. \n";
		patcher_error = 1;
	}
	load_uname();
}

void Patcher::load_uname()
{
#ifdef OBJ_NAME
	uname = OBJ_NAME;
#else
	cout<<fg::yellow<<"Assuming the uname to be default..."<<fg::reset<<endl;
	uname = "FaP.exe";
#endif
}

bool Patcher::check_system()
{
	if (system(nullptr))
	{
#ifdef DEBUG
		cout <<fg::green<< "Command processor exists "<<fg::reset<<endl;
		cout << " " "\n";
#endif
		return true;
	}
	else
	{
		cout <<fg::red<< "Command processor doesn't exist \n";
		return false;
	}
}

[[noreturn]] void debug_pause()
{
	cout<<"Press any key to continue..."<<endl;
	cin.clear();
	cin.ignore(numeric_limits<streamsize>::max(), '\n');
	cin.get();
	longjmp(jump_buffer, 1);
}

bool Patcher::check_gcc()
{
	if (!system("g++ --version"))
	{
#ifdef DEBUG
		cout <<fg::green<< "Detected g++"<<fg::reset<<endl;
		cout << " " "\n";
#endif
		return true;
	}
	else
	{
		return false;
	}
}

bool Patcher::check_make()
{
	if (!system("make --version"))
	{
#ifdef DEBUG
		cout <<fg::green<< "Detected make"<<fg::reset<<endl;
		cout << " " "\n";
#endif
		make = "make";
		return true;
	}
	else if(!system("mingw32-make --version"))
	{
#ifdef DEBUG
		cout <<fg::green<< "Detected mingw32-make"<<fg::reset<<endl;
		cout << " " "\n";
#endif
		make = "mingw32-make";
		return true;
	}
	else
	{
		cout <<fg::red<< "Make is not present on the system or is missing from the enviroment variable.\n";
		return false;
	}
}