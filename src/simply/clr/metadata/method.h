#pragma once

#include <string>
#include <simply/com/com_ptr.h>
#include "implementation/interop.h"
#include "token.h"

namespace simply { namespace clr { namespace metadata
{
    class method
    {
    public:
        method(const token token, com_ptr<IMetaDataImport2>);
        const std::wstring name() const;
        token token() const;
        bool operator==(const method&) const;

    private:
        const metadata::token _token;
        const com_ptr<IMetaDataImport2> _metadata;
    };
}}}
