
#include "patcher.hpp"

string make;
string uname;
bool patcher_error;

Patcher::Patcher(string filename_in, string filename_out)
{
	if (!boost::filesystem::exists(filename_in))
	{
		cout<<fg::red<<filename_in<<"not found! Rename the file if needed\n";
		patcher_error = 1;
	}
	else
	{
		boost::filesystem::copy_file(filename_in, filename_out, boost::filesystem::copy_option::overwrite_if_exists);
	}

	if(!boost::filesystem::exists("/build"))
	{
		boost::filesystem::create_directory("build");
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
	uname = "FaPatcher.exe";
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

int debug_pause()
{
	cout<<"Press any key to continue..."<<endl;
	cin.clear();
	cin.ignore(numeric_limits<streamsize>::max(), '\n');
	cin.get();
	exit(1);
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