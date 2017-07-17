#include <simply/com.h>
#include "metadata_enumerator.h"
#include "methods.h"

using namespace std;

namespace simply { namespace clr { namespace metadata { namespace implementation
{
    class method_enumerator : public metadata_enumerator<method>
    {
        const token type_token;
    public:
        method_enumerator(token type_token, com_ptr<IMetaDataImport2> metadata)
            : type_token { type_token }, metadata_enumerator { metadata }
        {
        }

        bool get_next(method* element) override
        {
            mdMethodDef method_token;
            unsigned long count;
            check(metadata->EnumMethods(&enum_handle, type_token, &method_token, 1, &count));
            if (count == 1)
            {
                new(element) method { token { method_token }, metadata };
                return true;
            }
            return false;
        }
    };

    methods::methods(token type_token, com_ptr<IMetaDataImport2> metadata)
        : metadata_enumerable<method>(metadata), type_token { type_token }
    {
    }

    unique_ptr<enumerator<method>> methods::create_enumerator()
    {
        return unique_ptr<enumerator<method>> { new method_enumerator(type_token, metadata) };
    }
}}}}