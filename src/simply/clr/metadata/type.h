#pragma once

#pragma warning(disable: 4091)
#include <cor.h>
#pragma warning(default: 4091)
#include <simply/com/com_ptr.h>
#include "assembly.h"

namespace simply { namespace clr { namespace metadata 
{
	class assembly;

    class type
    {
    public:
		type(mdTypeDef token, com_ptr<IMetaDataImport2> metadata);

		assembly assembly() const;
        const std::wstring name() const;
		unsigned int token() const;
		bool operator==(const type& other) const;

    private:
        const mdTypeDef _token;
        const com_ptr<IMetaDataImport2> _metadata;
        
		friend class type_test;
    };
}}}
