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
        const uint32_t value;
    public:
        token(table table, unsigned index);
        table table() const;
        unsigned index() const;
    };
}}}
