#pragma once

#include "system_type.h"
#include "type_signature.h"

namespace simply { namespace clr { namespace metadata { namespace implementation
{
    class system_type_signature : type_signature
    {
        const system_type _system_type;
    public:
        system_type_signature(system_type system_type) : _system_type(system_type)
        {
        }

        inline system_type system_type() const
        {
            return _system_type;
        }
    };
}}}}
