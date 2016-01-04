#include <string>
#include <stdexcept>
#include "token.h"

namespace simply { namespace clr { namespace metadata
{
    using table_enum = table; // to disambiguate from the table function
    const unsigned table_bit_offset { 24 };
    const unsigned index_bit_mask { 0xFFFFFF };

    token::token(table_enum table, unsigned index)
        : value { static_cast<uint32_t>(table) << table_bit_offset | index }
    {
        if (index > index_bit_mask)
        {
            throw std::out_of_range { "Metadata index is out of range: " + std::to_string(index) };
        }
    }

    table_enum token::table() const
    {
        return static_cast<table_enum>(value >> table_bit_offset);
    }

    unsigned token::index() const
    {
        return value & index_bit_mask;
    }
}}}
