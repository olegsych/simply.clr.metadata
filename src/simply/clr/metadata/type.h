#pragma once

#include <simply/com/com_ptr.h>
#include "assembly.h"
#include "implementation/interop.h"
#include "method.h"

namespace simply { namespace clr { namespace metadata
{
	class assembly;

    class type
    {
    public:
		type(mdTypeDef token, com_ptr<IMetaDataImport2> metadata);

		assembly assembly() const;
		range<method> methods() const;
        const std::wstring name() const;
		unsigned int token() const;
		bool operator==(const type& other) const;

    private:
        const mdTypeDef _token;
        const com_ptr<IMetaDataImport2> _metadata;

		friend class type_test;
    };
}}}
