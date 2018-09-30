
#include "utils.hpp"

int Utils::parse_offset(string filename, string expr)
{
	fstream file(filename);
	line.clear();
	offset = 0;
	pos = 0;
	if(file.is_open())
	{
		while(getline(file,line))
		{
			pos = line.find(expr);
			if(pos!=string::npos)
			{
				line = line.substr(pos);
				tok = strtok(&line[0], "0x");
				line.erase(0,strlen(tok)+1);
				line.insert(0,"0");
				offset = boost::lexical_cast<HexTo<int>>(line);
				file.close();
				return offset;
			}
		}
		cout<<fg::red<<"Could not find : "<<expr<<" in the file : "<<filename<<endl;
		cin.get();
		exit(1);
	}
	else
	{
		cout<<fg::red<<"Unable to open the file : "<<filename<<endl;
		cin.get();
		exit(1);
	}
	return 1;	
}

bool Utils::gpp_link(string filename, string command)
{
	command.append(" PRIME_NAME=");
	command.append(filename);
	filename = rem_extension(filename);
	string filename_tmp = filename.append(".tmp");
	string tmp_Prefix = " TMP_NAME=";
	tmp_Prefix.append(filename_tmp);
	command.append(tmp_Prefix);
	
	cout<<fg::cyan<<"GENERATING .TMP FILES ------------------------------->"<<fg::reset<<endl;
	//here convert the binary to MS PE format.
	if(system(&command[0]))
	{
		cout<<fg::red<<"Link error."<<endl;
		cin.get();
		exit(1);
	}
	cout<<"\n";
	
	command = "make rip_out_binary";
	command.append(tmp_Prefix);
	string filename_prime = " PRIME_NAME=";
	command.append(filename_prime);
	filename = rem_extension(filename);
	filename_prime = filename.append(".bin");
	command.append(filename_prime);
	
	cout<<fg::cyan<<"GENERATING .BIN FILES --------------------------->"<<fg::reset<<endl;
	
	if(system(&command[0]))
	{
		cout<<fg::red<<"Ripping binary error"<<endl;
		cin.get();
		exit(1);
	}
	cout<<"\n";
	return true;
}

int Utils::write_def_table(function_table &table)
{
	ofstream outfile ("preprocessor/define.h");
	
	for(int i=0; i<table.Name.size(); i++)
	{
		outfile<<"#define "<<table.Name[i]<<" 0x"<<hex<<table.FunctionVirtualAddress[i]<<endl;
	}
	outfile.close();
}

function_table Utils::linker_map_parser(string filename)
{
	function_table table;
	line.clear();
	offset = 0;
	pos = 0;
	
	ifstream map_file(filename);
	text_sec_found = false;
	if(map_file.fail()||!map_file)
	{
		cout <<fg::red<< "Cannot open map_file : " << filename <<endl;
		cin.get();
		exit(1);
	}
	table.section_alignment = parse_offset(filename, "__section_alignment__ = "); 	
	while(getline(map_file,line))
	{
		if(line.find("*(.text.*)")!=string::npos)
		{break;}
		if(line.find("0x")==string::npos)
		{continue;}
		if(line.find(".text")!=string::npos)
		{text_sec_found = true; continue;}
	
		if(text_sec_found == true)
		{
			if(line.find("../build/")==string::npos)
			{
				try
				{
					line.resize(line.find("("));
				}catch(const length_error &e)
				{
					cout<<fg::yellow<<"Skipping not a function in linker mapfile... "<<line<<fg::reset<<endl;
					continue;
				}
					
				pos = line.find("::");
				if(pos!=string::npos)
				{
					line.replace(pos,2,"__");
				}
				stringstream ss(line);
				while(ss>>word)
				{
					if(word.find("0x")!=string::npos)
					{
						offset = boost::lexical_cast<HexTo<int>>(word);
						table.FunctionVirtualAddress.push_back(offset-table.section_alignment);
					}
					else
					{
						table.Name.push_back(word);
					}
				}
			}
		}
	}	
	map_file.close();
	return table;
}

