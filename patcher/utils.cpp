
#include "utils.hpp"

namespace binPatcher
{

int Utils::parse_offset(FileIO& file, string expr)
{
	line.clear();
	offset = 0;
	pos = 0;
	while(getline(file._file,line))
	{
		pos = line.find(expr);
		if(pos!=string::npos)
		{
			line = line.substr(pos);
			tok = strtok(&line[0], "0x");
			line.erase(0,strlen(tok)+1);
			line.insert(0,"0");
			offset = boost::lexical_cast<HexTo<int>>(line);
			return offset;
		}
	}
	cout<<fg::red<<"Could not find : "<<expr<<" in the file : "<<file.filename<<endl;
	debug_pause();
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

#ifdef DEBUG
	cout<<fg::cyan<<"GENERATING .TMP FILES ------------------------------->"<<fg::reset<<endl;
#endif

	//here convert the binary to MS PE format.
	if(system(&command[0]))
	{
		cout<<fg::red<<"Link error."<<endl;
		cout<<"In function "<<__func__<<endl;
		debug_pause();
	}

#ifdef DEBUG
	cout<<"\n";
#endif

	command = make+" rip_out_binary";
	command.append(tmp_Prefix);
	string filename_prime = " PRIME_NAME=";
	command.append(filename_prime);
	filename = rem_extension(filename);
	filename_prime = filename.append(".bin");
	command.append(filename_prime);

#ifdef DEBUG
	cout<<fg::cyan<<"GENERATING .BIN FILES --------------------------->"<<fg::reset<<endl;
#endif

	if(system(&command[0]))
	{
		cout<<fg::red<<"Ripping binary error"<<endl;
		cout<<"In function "<<__func__<<endl;
		debug_pause();
	}
#ifdef DEBUG
	cout<<"\n";
#endif
	return true;
}

void Utils::write_def_table(function_table &table)
{
	ofstream outfile ("preprocessor/define.h");
#ifdef DEBUG
	cout<<"Writing define table:\n";
#endif
	size_t _size = table.Name.size();
	for(unsigned int i=0; i<_size; i++)
	{
#ifdef DEBUG
		cout<<"#define "<<table.Name[i]<<" 0x"<<hex<<table.FunctionVirtualAddress[i]<<endl;
#endif
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

	text_sec_found = false;
	FileIO map_file(filename);
	table.section_alignment = parse_offset(map_file, "__section_alignment__ = ");
	while(getline(map_file._file,line))
	{
		if(line.find("*(.text.*)")!=string::npos)
		{
			break;
		}
		if(line.find("0x")==string::npos)
		{
			continue;
		}
		if(line.find(".text")!=string::npos)
		{
			text_sec_found = true;
			continue;
		}

		if(text_sec_found == true)
		{
			if(line.find("../build/")==string::npos)
			{
				try
				{
					line.resize(line.find("("));
				}
				catch(const length_error &e)
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
						try
						{
							offset = boost::lexical_cast<HexTo<int>>(word);
						}
						catch(const bad_cast &bc)
						{
							cout<<fg::red<<"error: bad_cast "<<bc.what()<<fg::reset<<endl;
							debug_pause();
						}
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
	return table;
}

string Utils::cut_on_first_null(string line)
{
	try
	{
		line.resize(line.find(" "));
	}
	catch(const length_error &e)
	{
		cout<<fg::yellow<<__func__<<" Skipping instruction without operand. "<<line<<fg::reset<<endl;
	}
	return line;
}

string Utils::add_quotations(string line)
{
	line.insert(0,"\"");
	line.append(" \\n \" ");
	return line;
}

void Utils::write_gcc_asm(string dbg_inline_file, x64dbg_parser_struct &table)
{
	dbg_inline_file = rem_extension(dbg_inline_file);
	dbg_inline_file.append(".gas");
	ofstream outfile (dbg_inline_file);

	size_t _size = table.GccInstruction.size();
	for(unsigned int i=0; i<_size; i++)
	{
		outfile<<table.GccInstruction[i]<<endl;
	}
	outfile.close();
}

//hmmm, this wants to be a template function!
size_t Utils::inc_search(string target, string searched, size_t pos)
{
	return target.find_first_of(searched, pos);
}

x64dbg_parser_struct Utils::x64dbg_to_gcc_inline(string dbg_inline_file, int align_calls)
{
	if(!boost::filesystem::exists(dbg_inline_file))
	{
		ofstream outfile (dbg_inline_file);
		cout<<fg::yellow<<"File "<<dbg_inline_file<<" created."<<endl;
		cout<<"Nothing to be parsed."<<endl;
		debug_pause();
	}

	bool right_to_left_instr;
	bool not_hex_instr;
	bool comment;
	size_t comment_pos = 0;
	x64dbg_parser_struct parser_struct;
	line.clear();
	FileIO dbg_inline_file_obj(dbg_inline_file, ios::in);
	for(int i=0; getline(dbg_inline_file_obj._file,line); i++)
	{
		right_to_left_instr = false;
		not_hex_instr = false;
		comment = false;
		size_t orig_size = line.size();
		for(pos = 0; pos<orig_size; pos++)
		{
			if(line.at(pos) == ' ')
			{
				continue;
			}
			break;
		}
		if(pos == orig_size)
		{
			continue;
		}
		line = line.substr(pos);
		orig_size = orig_size - pos;
		
		// ; symbol is usually a comment in asm, so move it out of real code.
		pos = line.find(";");
		if (string::npos != pos)
		{
			comment = true;
			comment_pos = pos;
		}
		
		word = cut_on_first_null(line);
		try
		{
			pos = line.find(",");
			tmp_pos = line.find(".");
			
			if(pos > comment_pos && comment == true)
			{
				goto needs_more_processing;
			}		

			if(tmp_pos > comment_pos && comment == true)
			{
				goto needs_more_processing;
			}			
			
			if(pos!=string::npos && tmp_pos!=string::npos)
			{
				if(pos>tmp_pos)
				{
					line.erase(0,tmp_pos+1);
				}
				else
				{
					line.erase(pos+1,tmp_pos-pos);
					right_to_left_instr = true;
				}
			}
			else
			{
				line.erase(0,tmp_pos+1);
			}
							
			if (line.size() != orig_size)
			{
				if(!right_to_left_instr)
				{
					line.insert(0,"0x");
				}
				if(word.compare("call")==0 || word.find_first_of("j") != string::npos)
				{
					try
					{
						offset = boost::lexical_cast<HexTo<int>>(line);
					}
					catch(const bad_cast &bc)
					{
						cout<<fg::red<<"error: bad_cast "<<bc.what()<<fg::reset<<endl;
					}
					line = to_string(offset+align_calls);
					if(word.compare("call")==0)
					{
						parser_struct.GccInstruction.push_back(");");
						parser_struct.GccInstruction.push_back("__asm__ (");
					}
				}
				if(!right_to_left_instr)
				{
					word+=" ";
					word+=line;
				}
				else
				{
					goto needs_more_processing;
				}
				parser_struct.GccInstruction.push_back("	"+add_quotations(word));
				continue;
			}
		}
		catch(const out_of_range &e)
		{
			cout<<fg::yellow<<__func__<< "Skipping, no named hex address "<<line<<fg::reset<<endl;
		}

needs_more_processing:
		tmp_pos = line.find("var");
		if(tmp_pos != string::npos)
		{
			tmp_pos = line.find("dword ptr");
			if(tmp_pos != string::npos)
			{
				not_hex_instr = true;
				line.erase(tmp_pos, 9);
				line.insert(tmp_pos+2,"0x");
				goto skip_arg_check;
			}
			tmp_pos = line.find("byte ptr");
			if(tmp_pos != string::npos)
			{
				not_hex_instr = true;
				line.erase(tmp_pos, 8);
				line.insert(tmp_pos+2,"0x");
				goto skip_arg_check;
			}			
		}
		tmp_pos = line.find("arg");
		if(tmp_pos != string::npos)
		{
			tmp_pos = line.find("dword ptr");
			if(tmp_pos != string::npos)
			{
				not_hex_instr = true;
				line.erase(tmp_pos, 9);
				line.insert(tmp_pos+2,"0x");
				goto skip_arg_check;
			}
			tmp_pos = line.find("byte ptr");
			if(tmp_pos != string::npos)
			{
				not_hex_instr = true;
				line.erase(tmp_pos, 8);
				line.insert(tmp_pos+2,"0x");
				goto skip_arg_check;
			}				
		}
		tmp_pos = line.find("st");
		if(tmp_pos != string::npos)
		{
			pos = line.find("st(0x0),");
			if(pos!=string::npos)
			{
				line.erase(pos, 8);	
			}
			else
			{
				pos = line.find("st(0),");
				if(pos!=string::npos)
				{
					line.erase(pos, 6);	
				}
			}				
			pos = word.length();
			if(tmp_pos > pos)
			{			
				if(line.find("ptr") == string::npos)
				{
					not_hex_instr = true;
					goto skip_arg_check;
				}			
			}
			else
			{
				tmp_pos = line.find(",st(0x0)");
				if(tmp_pos!=string::npos)
				{
					line.erase(tmp_pos, 8);	
				}
				else
				{
					tmp_pos = line.find(",st(0)");
					if(tmp_pos!=string::npos)
					{
						line.erase(tmp_pos, 6);	
					}
				}		
				
				if(line.find("ptr") == string::npos)
				{
					if(line.find("test") == string::npos)
					{
						line.insert(pos, " dword ptr ");
						goto skip_arg_check;
					}
				}				
			}
		}	
skip_arg_check:
		pos = line.find_first_of("0123456789ABCDEF");
		for(int i=0; pos<=line.length(); i++)
		{
			pos = inc_search(line,"0123456789ABCDEF",pos+i);
			if (string::npos != pos)
			{
				_word = line;
				_word.erase(pos);
				_word = _word.back();
				if(string::npos == _word.find_first_of("abcdefghijklmnopqrstuvwxyz"))
				{
					size_t mem_x_pos = pos;
					if(line.at(pos) == '0')
					{
						try
						{
							if(line.at(pos+1) == 'x')
							{
								goto _0x_not_needed;
							}
						}
						catch(const out_of_range &e)
						{}
					}
					if(!not_hex_instr)
					{
						line.insert(pos,"0x");
					}
					_0x_not_needed:
					pos = inc_search(line,"h",pos);
					if(pos!=string::npos)
					{
						try
						{
							_word = line.substr(pos+1,orig_size);
						}
						catch(const out_of_range &e)
						{
							_word = line.substr(pos+1,line.length());
						}
						line.erase(pos); //erase urinary "h" at the end of hex value.
						line+=_word;
					}
					pos = line.find(",");
					if(pos!=string::npos && mem_x_pos<pos)
					{
						continue;
					}
					break;
				}
			}
			else
			{
				break;
			}
		}
		_word.clear();
		pos = line.find_first_of("[");
		if (string::npos != pos)
		{
			if(line.find("fs:")!=string::npos)
			{
				line.insert(pos+1, line.substr(pos-3,3));
			}
			if(line.at(pos-4) == ' ') //this is only for x64dbg disassembly
			{
				line.erase(pos-3,3);
			}
		}
		
		pos = line.find(": ; 0x");
		if(pos != string::npos)
		{
			tmp_pos = line.length();
			_word = line.substr(pos+6, tmp_pos);
			line.erase(pos, 3);
			line.insert(pos," = ");
			try
			{
				offset = boost::lexical_cast<HexTo<int>>(_word);
			}
			catch(const bad_cast &bc)
			{
				cout<<fg::red<<"error: bad_cast "<<bc.what()<<fg::reset<<endl;
			}	
			_word = to_string(offset+align_calls);
			line.replace(pos+6, tmp_pos-pos-6, _word);
			line.erase(pos+4, 2);
			parser_struct.GccInstruction.push_back("	"+add_quotations(line));
			continue;			
		}
		
		if(comment)
		{
			tmp_pos = line.length();
			signed int i = orig_size - tmp_pos;
			if(i == 0)
			{
				_word = line;
				_word.erase(0,comment_pos);
				line.erase(comment_pos, tmp_pos); 
				line = add_quotations(line);
				line.append("//!");
				line.append(_word);
				parser_struct.GccInstruction.push_back("	"+line);
				continue;				
			}
			else if(i < 0)
			{
				i = abs(i);
				_word = line;
				_word.erase(0,comment_pos+i);
				if(line.at(comment_pos) == ';')
				{
					line.erase(comment_pos, tmp_pos);
				}
				else
				{
					line.erase(comment_pos+i, tmp_pos);
				}			
				line = add_quotations(line);
				line.append("//!");
				line.append(_word);
				parser_struct.GccInstruction.push_back("	"+line);
				continue;					
			}
			else 
			{
				_word = line;
				_word.erase(0,comment_pos-i);
				if(line.at(comment_pos) == ';')
				{
					line.erase(comment_pos, tmp_pos);
				}
				else
				{
					line.erase(comment_pos-i, tmp_pos);
				}					
				line = add_quotations(line);
				line.append("//!");
				line.append(_word);
				parser_struct.GccInstruction.push_back("	"+line);
				continue;				
			}			
		}
		parser_struct.GccInstruction.push_back("	"+add_quotations(line));
	}

	parser_struct.GccInstruction.push_back(");");
	return parser_struct;
}

} //namespace binPatcher





