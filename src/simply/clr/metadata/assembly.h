#pragma once

#include <simply/com/com_ptr.h>
#include <simply/collections/range.h>
#include "assembly_identity.h"
#include "implementation/interop.h"
#include "token.h"
#include "type.h"

namespace simply { namespace clr { namespace metadata
{
    class token;
    class type;

    class assembly
    {
    public:
        assembly_identity identity() const;
        token token() const;
        range<type> types() const;
        bool operator==(const assembly& other) const;

        static assembly load_from(const std::wstring& file_path);

    private:
        const com_ptr<IMetaDataAssemblyImport> _metadata;

        assembly(com_ptr<IMetaDataAssemblyImport> metadata);

        friend class type;
        friend class assembly_test;
    };
}}}
