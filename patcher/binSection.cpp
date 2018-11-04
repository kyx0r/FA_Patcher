
#include "binSection.hpp"

image_section_header BinSection::populate_image_section_header(const string &filename)
{
	image_section_header header;
	FileIO pe_file(filename, ios::in | ios::binary);
	try
	{
		pe_base image(pe_factory::create_pe(pe_file._file));
		#ifdef DEBUG
		cout << "Reading PE sections..." << hex << showbase << endl << endl;
		#endif
		const section_list sections(image.get_image_sections());
		for(section_list::const_iterator it = sections.begin(); it != sections.end(); ++it)
		{
			const section& s = *it; 
			#ifdef DEBUG
 			cout << "Section [" << s.get_name() << "]" <<endl 
				<< "Size of raw data: " << s.get_size_of_raw_data() <<endl 
				<< "Virtual address: " << s.get_virtual_address() <<endl 
				<< "Virtual size: " << s.get_virtual_size() <<endl 
				<<endl; 
			cout<<"End of PE sections dump..."<<dec<<endl;
			#endif
			header.Name.push_back(s.get_name());
			header.VirtualSize.push_back(s.get_virtual_size());
			header.VirtualAddress.push_back(s.get_virtual_address()); 
			header.SizeOfRawData.push_back(s.get_size_of_raw_data()); 
		}
	}
	catch(const pe_exception& e)
	{
		std::cout <<fg::red<< "Error: " << e.what() << std::endl;
		cout<<"In function "<<__func__<<endl;
		debug_pause();
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
		cout<<"In function "<<__func__<<endl;
		debug_pause();
	}
	rebuild_pe(image, new_pe_file);
		
	#ifdef DEBUG	
	cout <<fg::green<<"PE was rebuilt and saved to " << out_file_name <<fg::reset<<endl; 	
	#endif
	
	return true;
}

void BinSection::apply_Ext(const int verisign_offset, FileIO& fa)
{
	//	const int verisign_offset = 0xBDD000; //.ext
	//const int verisign_offset = 0xBDF000; //.exxt
	align_data = 0;
	align_rdata = 0;
	align_bss = 0;
	align_idata = 0;
	
	if(system(&ext_sector[0]))
	{
		cout<<fg::red<<"Error when calling ext_sector "<<endl;
		cout<<"In function "<<__func__<<endl;
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
	
	#ifdef DEBUG
	cout<<__func__<<": "<<make_ext_gpp_link<<endl;
	#endif

	gpp_link("build/exxt_sector.o", make_ext_gpp_link);
	
	FileIO exxt("build/exxt_sector.bin", ios::in|ios::binary|ios::ate);
	
	size = exxt.get_file_size();
	
	#ifdef DEBUG
	cout<<fg::magenta<<"APPLY .EXT SECTION   Number of instructions: "<<size<<fg::reset<<endl;
	#endif
	
	fa.fWriteBinaryFile(exxt.fReadBinaryFile(), verisign_offset, size);
}

