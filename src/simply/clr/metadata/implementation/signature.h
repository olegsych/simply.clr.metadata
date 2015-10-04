#pragma once

#include <cstdint>
#include <functional>

namespace simply { namespace clr { namespace metadata { namespace implementation
{
	class signature
	{
		const uint8_t* current;
		const uint8_t* const end;
	public:
		signature(const void* begin, const void* end);
        friend struct read_byte;
	};

    struct read_byte
    {
        static std::uint8_t value(signature&);
    };

    template<typename read_byte = read_byte>
    struct read_unsigned_integer
    {
        static std::uint32_t value(signature& signature)
        {
            uint8_t first = read_byte::value(signature);

            if ((first & 0xC0) == 0xC0)
            {
                uint8_t second = read_byte::value(signature);
                uint8_t third = read_byte::value(signature);
                uint8_t fourth = read_byte::value(signature);
                return ((first & 0x3F) << 24) + (second << 16) + (third << 8) + fourth;
            }

            if ((first & 0x80) == 0x80)
            {
                uint8_t second = read_byte::value(signature);
                return ((first & 0x7F) << 8) + second;
            }

            return first;
        }
    };
}}}}
