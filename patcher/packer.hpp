#include "patcher.hpp"
#include "fileIO.hpp"

// the magic as a string is: KJPK
#define PACK_MAGIC 0x4B4A414B

#ifdef defined(WIN32) || defined(_WIN32) || defined(_WIN64)

#else
	
#endif
 
void packDataFiles( const char* fileName, const char* execName );
const char* getPackDataFile();
// an object that holds the magic value.
class Packer
{
public:
   PackEnd( uint32_t offset ) { mMagic = PACK_MAGIC; mOffset = offset; }
   PackEnd() { ; }
   uint32_t mMagic;
   uint32_t mOffset;
};