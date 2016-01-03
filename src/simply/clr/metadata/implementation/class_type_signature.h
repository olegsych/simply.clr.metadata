#pragma once

#include "signature.h"
#include "type_signature.h"

namespace simply { namespace clr { namespace metadata { namespace implementation
{
    class class_type_signature : type_signature
    {
        const uint32_t _type_token;
    public:
        class_type_signature(signature& signature)
            : _type_token(signature.read_type_token())
        {
        }

        inline uint32_t type_token() const
        {
            return _type_token;
        }
    };
}}}}
