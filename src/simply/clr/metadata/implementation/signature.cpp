#include <stdexcept>
#include "interop.h"
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

	uint8_t read_byte(blob_cursor& blob)
	{
		if (blob.current < blob.end)
		{
			return *blob.current++;
		}

		throw out_of_range { "attempting to read past end of signature." };
	}

    std::uint32_t read_unsigned_integer(blob_cursor& blob)
    {
        uint8_t first = read_byte(blob);

        if ((first & 0xC0) == 0xC0)
        {
            uint8_t second = read_byte(blob);
            uint8_t third = read_byte(blob);
            uint8_t fourth = read_byte(blob);
            return ((first & 0x3F) << 24) + (second << 16) + (third << 8) + fourth;
        }

        if ((first & 0x80) == 0x80)
        {
            uint8_t second = read_byte(blob);
            return ((first & 0x7F) << 8) + second;
        }

        return first;
    }

    std::uint32_t read_type_token(blob_cursor& blob)
    {
        uint32_t encoded { read_unsigned_integer(blob) };
        uint32_t token_type { encoded & 0x03 };
        uint32_t token_value { encoded >> 2 };
        switch (token_type)
        {
            case 0: return CorTokenType::mdtTypeDef | token_value;
            case 1: return CorTokenType::mdtTypeRef | token_value;
            case 2: return CorTokenType::mdtTypeSpec | token_value;
            default: throw logic_error { "Unexpected token type: 3." };
        }
    }
}}}}
