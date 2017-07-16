#include "method.h"
#include <stdexcept>
#include <simply/com.h>

using namespace std;

namespace simply { namespace clr { namespace metadata 
{
    method::method(mdMethodDef token, com_ptr<IMetaDataImport2> metadata)
        : _token { token }, _metadata { metadata }
    {
        if (!metadata.get())
        {
            throw invalid_argument { "metadata must not be a nullptr." };
        }
    }

    const wstring method::name() const
    {
        const unsigned long max_length = 1024;
        wchar_t buffer[max_length];
        unsigned long length;
        check(_metadata->GetMethodProps(_token, nullptr, buffer, max_length, &length, nullptr, nullptr, nullptr, nullptr, nullptr));
        return wstring { buffer, length };
    }

    unsigned int method::token() const
    {
        return _token;
    }

    bool method::operator==(const method& other) const
    {
        return _token == other._token
            && _metadata.get() == other._metadata.get();
    }
}}}