#include "Utilities.h"

namespace Util
{

	unsigned int fnvHash(const char* str)
	{
		const size_t length = strlen(str) + 1;
		unsigned int hash = OFFSET_BASIS;
		for (size_t i = 0; i < length; ++i)
		{
			hash ^= *str++;
			hash *= FNV_PRIME;
		}
		return hash;
	}

}