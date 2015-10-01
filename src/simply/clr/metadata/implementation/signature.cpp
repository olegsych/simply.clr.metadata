#include <stdexcept>
#include "signature.h"

using namespace std;

namespace simply { namespace clr { namespace metadata { namespace implementation 
{
	signature::signature(const void* begin, const void* end)
		: current { reinterpret_cast<const uint8_t*>(begin) }, end { reinterpret_cast<const uint8_t*>(end) }
	{
		if (!begin)
		{
			throw invalid_argument { "begin must not be a nullptr." };
		}

		if (!end)
		{
			throw invalid_argument { "end must not be a nullptr." };
		}

		if (end < begin)
		{
			throw invalid_argument { "begin must point before end." };
		}
	}

	uint8_t signature::read_byte()
	{
		if (current < end)
		{ 
			return *current++;
		}

		throw out_of_range { "attempting to read past end of signature." };
	}

	uint32_t signature::read_unsigned_integer()
	{
		uint8_t first = read_byte();

		if ((first & 0xC0) == 0xC0)
		{
			uint8_t second = read_byte();
			uint8_t third = read_byte();
			uint8_t fourth = read_byte();
			return ((first & 0x3F) << 24) + (second << 16) + (third << 8) + fourth;
		}

		if ((first & 0x80) == 0x80)
		{
			uint8_t second = read_byte();
			return ((first & 0x7F) << 8) + second;
		}

		return first;
	}
}}}}