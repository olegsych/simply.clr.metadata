#pragma once

#include "../token.h"
#include "signature.h"
#include "type_signature.h"

namespace simply { namespace clr { namespace metadata { namespace implementation
{
    class custom_type_signature : public type_signature
    {
        const token _token;
    public:
        custom_type_signature(const token token)
            : _token(token)
        {
        }

        inline token token() const
        {
            return _token;
        }
    };
}}}}
