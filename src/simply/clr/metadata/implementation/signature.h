#pragma once

#include <cstdint>

namespace simply { namespace clr { namespace metadata { namespace implementation
{
    class signature
    {
        const uint8_t* current;
        const uint8_t* const end;
    public:
        signature(const void* begin, const void* end);

        std::uint8_t read_byte();
        std::uint32_t read_unsigned_integer();
        std::uint32_t read_type_token();
    };
}}}}
