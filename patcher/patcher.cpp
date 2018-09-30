
#include "patcher.hpp"

Patcher::Patcher(const string& filename)
{
	if (!boost::filesystem::exists(filename))
	{
		cout<<fg::red<<filename<<"not found! Rename the file if needed\n";
		cin.get();
		exit(1);
	}
	
	if(!boost::filesystem::exists("/build"))
	{boost::filesystem::create_directory("build");}
		
	if(!check_system())
	{
		cout<<fg::red<<"No system present. Exiting patcher."<<endl;
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