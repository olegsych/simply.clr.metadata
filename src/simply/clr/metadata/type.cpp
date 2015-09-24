#include "type.h"
#include <simply/com.h>

using namespace std;

namespace simply { namespace clr { namespace metadata 
{
    type::type(mdTypeDef token, com_ptr<IMetaDataImport2> metadata)
        : _token {token}, _metadata { metadata }
    {
        if (!metadata.get())
        {
            throw invalid_argument("metadata must not be a nullptr.");
        }
    }

    assembly type::assembly() const
    {
		com_ptr<IMetaDataAssemblyImport> assembly_metadata;
		check(_metadata->QueryInterface(IID_IMetaDataAssemblyImport, assembly_metadata));
		return metadata::assembly { assembly_metadata };
    }

    const wstring type::name() const
    {
		const unsigned long max_length = 1024;
        wchar_t buffer[max_length];
        unsigned long actual_length;
        check(_metadata->GetTypeDefProps(_token, buffer, max_length, &actual_length, nullptr, nullptr));
        return wstring { buffer, actual_length };
    }

	unsigned int type::token() const
	{
		return _token;
	}

	bool type::operator==(const type& other) const
	{
		return _token == other._token 
			&& _metadata.get() == other._metadata.get();
	}
}}}