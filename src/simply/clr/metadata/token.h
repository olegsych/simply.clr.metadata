#pragma once

#include <cstdint>
#include "table.h"

namespace simply { namespace clr { namespace metadata
{
    /// <summary>
    /// A CLR metadata token.
    /// </summary>
    class token
    {
    public:
        token(table table, unsigned index);
        explicit token(uint32_t value);
        table table() const;
        unsigned index() const;
        operator uint32_t() const;
    private:
        const uint32_t value;
    };
}}}
