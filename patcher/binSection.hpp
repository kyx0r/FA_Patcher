
#include "patcher.hpp"
#include "fileIO.hpp"
#include "utils.hpp"

struct image_section_header
{
	vector <string> Name;
	vector <uint32_t> VirtualSize;
	vector <uint32_t> VirtualAddress;
	vector <uint32_t> SizeOfRawData;
};

namespace binPatcher
{

class BinSection : public Utils
{
public:
	bool create_Section(istream& pe_file, string out_file_name, const string& section_name, int raw_size = 1, int virtual_size = 0x1000);
	bool extend_Section(istream& pe_file, string out_file_name, const string& section_name, int raw_size, int virtual_size);
	bool remove_last_Section(istream& pe_file, string out_file_name);
	void apply_Ext(const int verisign_offset, FileIO& fa);

private:
	image_section_header populate_image_section_header(const string &filename);

	string make_ext_gpp_link = make+" ext_gpp_link";
	string ext_sector = make+" ext_sector";
	uint32_t align_data = 0;
	uint32_t align_rdata = 0;
	uint32_t align_bss = 0;
	uint32_t align_idata = 0;
	int size;

};

} //namespace binPatcher