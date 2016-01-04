#pragma once

#include "../token.h"
#include "signature.h"
#include "type_signature.h"

namespace simply { namespace clr { namespace metadata { namespace implementation
{
    class class_type_signature : type_signature
    {
        const token _token;
    public:
        class_type_signature(signature& signature)
            : _token(signature.read_type_token())
        {
        }

        inline token token() const
        {
            return _token;
        }
    };
}}}}
