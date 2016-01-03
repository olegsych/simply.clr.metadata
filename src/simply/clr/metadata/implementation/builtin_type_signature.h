#pragma once

#include "builtin_type.h"
#include "type_signature.h"

namespace simply { namespace clr { namespace metadata { namespace implementation
{
    class builtin_type_signature : type_signature
    {
        const builtin_type _builtin_type;
    public:
        builtin_type_signature(builtin_type builtin_type) : _builtin_type(builtin_type)
        {
        }

        inline builtin_type builtin_type() const
        {
            return _builtin_type;
        }
    };
}}}}
