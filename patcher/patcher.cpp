
#include "patcher.hpp"

Patcher::Patcher(const string& filename_in, const string& filename_out)
{
	cin.ignore(numeric_limits<streamsize>::max(),'\n');
	
	if (!boost::filesystem::exists(filename_in))
	{
		cout<<fg::red<<filename_in<<"not found! Rename the file if needed\n";
		cin.get();
		exit(1);
	}
	
	boost::filesystem::copy_file(filename_in, filename_out, boost::filesystem::copy_option::overwrite_if_exists);	
	
	if(!boost::filesystem::exists("/build"))
	{boost::filesystem::create_directory("build");}
		
	if(!check_system())
	{
		cout<<fg::red<<"No system present. Exiting patcher. \n";
		cin.get();
		exit(1);
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