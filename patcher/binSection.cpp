
#include "binSection.hpp"

FileIO io;

image_section_header BinSection::populate_image_section_header(const string &filename)
{
	image_section_header header;
	ifstream pe_file(filename, ios::in | ios::binary);
	if(!pe_file)
	{
		cout <<fg::red<< "Cannot open " << filename <<endl;
		cin.get();
		exit(1);
	}	
	
	try
	{
		pe_base image(pe_factory::create_pe(pe_file));
		//cout << "Reading PE sections..." << hex << showbase << endl << endl;
		const section_list sections(image.get_image_sections());
		for(section_list::const_iterator it = sections.begin(); it != sections.end(); ++it)
		{
			const section& s = *it; 
/* 			std::cout << "Section [" << s.get_name() << "]" << std::endl 
				<< "Size of raw data: " << s.get_size_of_raw_data() << std::endl 
				<< "Virtual address: " << s.get_virtual_address() << std::endl 
				<< "Virtual size: " << s.get_virtual_size() << std::endl 
				<< std::endl; */
			header.Name.push_back(s.get_name());
			header.VirtualSize.push_back(s.get_virtual_size());
			header.VirtualAddress.push_back(s.get_virtual_address()); 
			header.SizeOfRawData.push_back(s.get_size_of_raw_data()); 
		}
	}
	catch(const pe_exception& e)
	{
		std::cout <<fg::red<< "Error: " << e.what() << std::endl;
		cin.get();
		exit(1);
	}
	
	return header;
}

bool BinSection::create_Section(istream& pe_file, string out_file_name, const string& section_name, int raw_size, int virtual_size)
{
	pe_base image(pe_factory::create_pe(pe_file));
		
	section new_section;
	new_section.readable(true).writeable(true).executable(true).code(true);
	new_section.set_name(section_name); 
	new_section.resize_raw_data(raw_size);
		
	section& added_section = image.add_section(new_section);
	image.set_section_virtual_size(added_section, virtual_size);
		
	string::size_type slash_pos;
	if((slash_pos = out_file_name.find_last_of("/\\")) != string::npos)
	{
		out_file_name = out_file_name.substr(slash_pos + 1);
	}
		
	ofstream new_pe_file(out_file_name.c_str(), ios::out | ios::binary | ios::trunc);
	if(!new_pe_file)
	{
		cout <<fg::red<< "Cannot create " << out_file_name <<endl;
		cin.get();
		exit(1);
		return false;
	}
	rebuild_pe(image, new_pe_file);
		
	cout <<fg::green<<"PE was rebuilt and saved to " << out_file_name <<fg::reset<<endl; 	
	
	return true;
}

void BinSection::apply_Ext(const int verisign_offset)
{
	//	const int verisign_offset = 0xBDD000; //.ext
	//const int verisign_offset = 0xBDF000; //.exxt
	align_data = 0;
	align_rdata = 0;
	align_bss = 0;
	align_idata = 0;
	if(system("make ext_sector"))
	{
		cout<<fg::red<<"Error when calling ext_sector "<<endl;
		cin.get();
		exit(1);
	}
	
	image_section_header header = populate_image_section_header("build/exxt_sector.o");
	
	//There might be a better way of doing this, but for now all I got. 
	//This should be very realiable still, cause locations will be the same as original binary. 
	for(int i=0; i<header.Name.size(); i++)
	{
		if(header.Name[i].compare(".data") == 0)
		{
			align_data = header.VirtualAddress[i];
			make_ext_gpp_link.append(" align_data=" + to_string(align_data));
		}
		if(header.Name[i].compare(".rdata") == 0)
		{
			align_rdata = header.VirtualAddress[i]-align_data;
			make_ext_gpp_link.append(" align_rdata=" + to_string(align_rdata));
		}
		if(header.Name[i].compare(".bss") == 0)
		{
			align_bss = header.VirtualAddress[i]-align_rdata;
			make_ext_gpp_link.append(" align_bss=" + to_string(align_bss));
		}
		if(header.Name[i].compare(".idata") == 0)
		{
			align_idata = header.VirtualAddress[i]-align_bss;
			make_ext_gpp_link.append(" align_idata=" + to_string(align_idata));
		}
	}

	gpp_link("build/exxt_sector.o", make_ext_gpp_link);
	ext_F = io.fReadBinaryFile("build/exxt_sector.bin");
	
	cout<<fg::magenta<<"APPLY .EXT SECTION   Number of instructions: "<<io.get_file_size("build/exxt_sector.bin")<<fg::reset<<endl;
	io.fWriteBinaryFile("ForgedAlliance_exxt.exe", ext_F, verisign_offset, io.get_file_size("build/exxt_sector.bin"));
}

