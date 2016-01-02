#pragma once

#pragma warning(disable: 4091)
#include <cor.h>
#pragma warning(default: 4091)
#include <cstdint>

namespace simply { namespace clr { namespace metadata { namespace implementation
{
	class blob_cursor
	{
		const uint8_t* current;
		const uint8_t* const end;
	public:
		blob_cursor(const void* begin, const void* end);
        friend struct read_byte;
	};

    struct read_byte
    {
        static std::uint8_t value(blob_cursor&);
    };

    template<typename read_byte = read_byte>
    struct read_unsigned_integer
    {
        static std::uint32_t value(blob_cursor& blob)
        {
            uint8_t first = read_byte::value(blob);

            if ((first & 0xC0) == 0xC0)
            {
                uint8_t second = read_byte::value(blob);
                uint8_t third = read_byte::value(blob);
                uint8_t fourth = read_byte::value(blob);
                return ((first & 0x3F) << 24) + (second << 16) + (third << 8) + fourth;
            }

            if ((first & 0x80) == 0x80)
            {
                uint8_t second = read_byte::value(blob);
                return ((first & 0x7F) << 8) + second;
            }

            return first;
        }
    };

    template<typename read_unsigned_integer>
    struct read_type_token
    {
        static std::uint32_t value(blob_cursor& blob)
        {
            uint32_t encoded { read_unsigned_integer::value(blob) };
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
    };
}}}}
