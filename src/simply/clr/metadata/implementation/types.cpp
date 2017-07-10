#include <simply/com.h>
#include "metadata_enumerator.h"
#include "types.h"

using namespace std;

namespace simply { namespace clr { namespace metadata { namespace implementation
{
    class type_enumerator : public metadata_enumerator<type>
    {
    public:
        type_enumerator(com_ptr<IMetaDataImport2> metadata)
            : metadata_enumerator<type>(metadata)
        {
        }

        bool get_next(type* element) override
        {
            mdTypeDef token;
            ULONG count;
            check(metadata->EnumTypeDefs(&enum_handle, &token, 1, &count));
            if (count == 1)
            {
                new(element) type { metadata::token { token }, metadata };
                return true;
            }

            return false;
        }
    };

    types::types(com_ptr<IMetaDataImport2> metadata)
        : metadata_enumerable<type>(metadata)
    {
    }

    unique_ptr<enumerator<type>> types::create_enumerator()
    {
        return unique_ptr<enumerator<type>> { new type_enumerator { metadata } };
    }
}}}}
