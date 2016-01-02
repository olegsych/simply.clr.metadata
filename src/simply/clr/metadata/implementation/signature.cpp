#include <stdexcept>
#include "signature.h"

using namespace std;

namespace simply { namespace clr { namespace metadata { namespace implementation
{
    blob_cursor::blob_cursor(const void* begin, const void* end)
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

	uint8_t read_byte::value(blob_cursor& blob)
	{
		if (blob.current < blob.end)
		{
			return *blob.current++;
		}

		throw out_of_range { "attempting to read past end of signature." };
	}
}}}}
