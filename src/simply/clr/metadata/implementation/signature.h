#pragma once

#include <cstdint>

namespace simply { namespace clr { namespace metadata { namespace implementation
{
	class blob_cursor
	{
		const uint8_t* current;
		const uint8_t* const end;
	public:
		blob_cursor(const void* begin, const void* end);
        friend std::uint8_t read_byte(blob_cursor&);
	};

    std::uint8_t read_byte(blob_cursor&);
    std::uint32_t read_unsigned_integer(blob_cursor&);
    std::uint32_t read_type_token(blob_cursor&);
}}}}
