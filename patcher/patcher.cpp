
#include "patcher.hpp"

Patcher::Patcher(const string& filename_in, const string& filename_out)
{
	if (!boost::filesystem::exists(filename_in))
	{
		cout<<fg::red<<filename_in<<"not found! Rename the file if needed\n";
		debug_pause();
	}
	
	boost::filesystem::copy_file(filename_in, filename_out, boost::filesystem::copy_option::overwrite_if_exists);	
	
	if(!boost::filesystem::exists("/build"))
	{boost::filesystem::create_directory("build");}
		
	if(!check_system())
	{
		cout<<fg::red<<"No system present. Exiting patcher. \n";
		debug_pause();
	}
	
	if(!check_make())
	{
		cout<<fg::red<<"No make present. Exiting patcher. \n";
		debug_pause();
	}
}

bool Patcher::check_system()
{
	if (system(nullptr))
	{
		cout <<fg::green<< "Command processor exists "<<fg::reset<<endl;
		cout << " " "\n";
		return true;
	}
	else
	{
		cout <<fg::red<< "Command processor doesn't exist \n";
		return false;
	}
}  

void debug_pause()
{
	cout<<"Press any key to continue..."<<endl;
	cin.clear();
	cin.ignore(numeric_limits<streamsize>::max(), '\n');
	cin.get();
	exit(1);
}

bool Patcher::check_make()
{
	if (!system("make"))
	{
		cout <<fg::green<< "Detected make"<<fg::reset<<endl;
		cout << " " "\n";
		return true;
	}
	else if(!system("mingw32-make"))
	{
		cout <<fg::green<< "Detected mingw32-make"<<fg::reset<<endl;
		cout << " " "\n";
		return true;
	}
	else
	{
		cout <<fg::red<< "Make is not present on the system or is missing from the enviroment variable.\n";
		return false;
	}
}