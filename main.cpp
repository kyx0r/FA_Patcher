

#include <iostream>
//#include <string>
#include <cstring>
#include <limits>
#include <time.h>
#include <stdio.h>
#include <vector>

#include <assert.h>
//#include <filesystem>

#include <boost/thread/thread.hpp>
#include <boost/filesystem.hpp>

using namespace std;

inline char*ReadBinaryFile(const char*f)
{
	FILE*F = fopen(f,"rb");
	if(!F)
	{
		printf("\nCan't find \"%s\".\n",f);
		exit(1);
	}
	unsigned n;
	fseek(F,0,SEEK_END);
	n = ftell(F);
	rewind(F);
	char*s = new char[n+1];
	fread(s,1,n,F);
	fclose(F);
	s[n]=0;
	return s;
}

inline unsigned WriteBinaryFile(const char*f,const char*s, const int a, int bytes, const char*m="r+wb")
{
	FILE*F = fopen(f,m);
	fseek(F, a, SEEK_SET);
	cout<<"Patch in: "<<hex<<a<<endl;
	if(!F)
	{
		printf("\nCan't open \"%s\".\n",f);
		cin.get();
		exit(1);
	}
	unsigned n = fwrite(s,sizeof(char),bytes,F);
	fclose(F);
	cout<<"Number of instructions written: "<<n<<endl;
	return n;
}

inline vector<char*> Parse(char*s,const char*d=" ,\t\n\f\r"){
std::vector<char*>V;
for(s=strtok(s,d); s; s=strtok(0,d))
{
	V.push_back(s);
}
return V;
}

bool check_syscall()
{
	if (system(NULL))
	{
		cout << "Command processor exists \n";
		cout << " " "\n";
		return true;
	}
	else
	{
		cout << "Command processor doesn't exists \n";
		return false;
	}
}  

bool gpp_link()
{
	//here convert the binary to MS PE format.
	if(system("make gpp_lick"))
	{
		printf("Link error. \"%s\"\n");
		return false;
	}
	cout<<"Ripping binary from elf \n";
	if(system("make rip_out_binary"))
	{
		printf("Ripping binary  error. \"%s\"\n");
		return false;
	}
	return true;
}

bool gpp_Compile()
{
	if(!boost::filesystem::exists("\build"))
	{
		boost::filesystem::create_directory("build");
	}
	if(!boost::filesystem::exists("\hooks"))
	{
		boost::filesystem::create_directory("hooks");
	}
	if(!boost::filesystem::exists("\sections"))
	{
		boost::filesystem::create_directory("sections");
	}
	
	if(!check_syscall())
	{
		printf("\No system calls present. Exiting patcher. \"%s\".\n");
		cin.get();
		exit(1);
	}
	
	const int verisign_offset = 0xBDD000;
	char verisign_size = 0x1500;
	int section_size = 1500;
	cout<<"Patching the verisign code \n";
	WriteBinaryFile("ForgedAlliance_exxt.exe", &verisign_size, verisign_offset, 8);
	
	system("make ext_sector");
	gpp_link();
	boost::filesystem::path p("\hooks");
	boost::filesystem::directory_iterator end_itr;
	cout<<""<<endl;
	cout<<"Available hooks : \n";
	for (boost::filesystem::directory_iterator itr(p); itr != end_itr; ++itr)
    {
        if (boost::filesystem::is_regular_file(itr->path())) 
		{
            string current_file = itr->path().string();
            //cout << current_file << endl;
			size_t pos = current_file.find("hook_");
			if (pos!=string::npos)
			{
				string end = current_file.substr (pos);
				string Final_Filename;
				Final_Filename.append(end);
				cout<<Final_Filename<<endl;
			}
        }
	}
	//FILE*ext_F = fopen("build\ext_sector.bin", "rb");
	
	char *ext_F = ReadBinaryFile("build/ext_sector.bin");
	
	WriteBinaryFile("ForgedAlliance_exxt.exe", ext_F, verisign_offset, section_size);
}

bool init_Ext()
{
	char PE_header_values[10][15] = {"\x07\x00\x1F\x49\x4A\x4A\x00\x00","\x00\xC5\xE8\x00\x00\x10\x00\x00",
	"\xAC\x24\xBE\x00\x02\x00\x00\x00","\xF0\xE3\x00\xEC\xB8\x04\x00",
	"\x00\x00\x00\x00\x00\x00\x00\x00","\x2E\x65\x78\x74\x00\x00\x00\x00",
	"\x00\x15\x00\x00\x00\xB0\xE8\x00","\x00\x15\x00\x00\x00\xD0\xBD\x00",
	"\x00\x00\x00\x00\x00\x00\x00\x00","\x00\x00\x00\x00\x20\x00\x00\x60"};
	
	
	/* unsigned char PE_header_values[] = {0x07, 0xC5, 0xAC24, 0xF0E300ECB80400, 0x00, 0xB0, 0xE8, 0x00}; 
	size_t count = sizeof(PE_header_values);  */
	
	//char test[][10] = {"asdasdff","gggg"};
	//cout<<sizeof(test[0][10])<<endl;
	
/* 	string PE_header_values [10] = {"\x07","\xC5","\xAC\x24","\xF0\xE3\x00\xEC\xB8\x04\x00","\x00\x00\x00\x00\x00\x00\x00\x00",
	"\x2E\x65\x78\x74\x00\x00\x00\x00","\x00\x15\x00\x00\x00\xB0\xE8\x00","\x00\x15\x00\x00\x00\xD0\xBD\x00","\x00\x00\x00\x00\x00\x00\x00\x00","\x00\x00\x00\x00\x20\x00\x00\x60"}; 
	
	
	vector <string> _arr(PE_header_values, PE_header_values+10); */
	
	//PE_header_values.push_back("\x06");
	
	const int PE_header_address [] = {0x136, 0x180, 0x188, 0x1B9, 0x1C8, 0x318, 0x320, 0x328, 0x330, 0x338};
	

	for(int i=0; i<=sizeof(PE_header_address)/sizeof(PE_header_address[0]); i++)
	{	
		WriteBinaryFile("ForgedAlliance_exxt.exe", PE_header_values[i], PE_header_address[i], 8); //hardcoded for 8 bytes.
	}
	
	//const int PE_header_values [] = {0x136, 0x181, 0x188, 0x1B9, 0x1C8, 0x318};
	return true;
}

int main (void)
{
	//pFile = fopen ( "ForgedAlliance_ext.exe" , "wb" );
	//fputs ( "This is an apple." , pFile );
	//fseek ( pFile , SEEK_SET , SEEK_SET );
	//fputs ( " sam" , pFile );
	if (!boost::filesystem::exists("ForgedAlliance_base.exe"))
	{
		cout<<"ForgedAlliance_base.exe not found! Rename the file if needed\n";
	}
		
	boost::filesystem::copy_file("ForgedAlliance_base.exe", "ForgedAlliance_exxt.exe",boost::filesystem::copy_option::overwrite_if_exists);
	
	init_Ext();
	
	gpp_Compile();
	
//	system("g++"); 
	
	
	
	
/* 	vector<char*> A = Parse("ForgedAlliance_ext.exe");
	printf("TEXT PARSED USING Parse():\n");
	for(int i=0,m=A.size();i<m;++i)printf("%s%s",A[i],i==m-1?"\n":" , "); */
	
/*  	for(;;)
	{
	char *s = ReadTextFile("ForgedAlliance_ext.exe");
	cout<<s<<endl;
	} */
	
	cin.get(); 

	return 0;
	
}
