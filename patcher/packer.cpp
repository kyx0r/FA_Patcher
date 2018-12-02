#include "packer.hpp"


#ifdef defined(WIN32) || defined(_WIN32) || defined(_WIN64)
extern char binary_data_txt_start;
extern const void* binary_data_txt_size;
const size_t len = (size_t)&binary_data_txt_size;
#else
extern char _binary_data_txt_start;
extern const void* _binary_data_txt_size;
const size_t len = (size_t)&_binary_data_txt_size;
#endif

const char* getPackDataFile()
{
 
}

void packDataFiles( const char* fileName, const char* execName )
{

}