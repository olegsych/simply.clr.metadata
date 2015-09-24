#pragma once

#include <ostream>

namespace simply { namespace clr { namespace metadata
{
    enum class hash_algorithm
    {
        none,
        md5,
        sha1,
        sha256,
        sha384,
        sha512
    };

    template <class char_t, class traits>
    std::basic_ostream<char_t, traits>& operator<<(std::basic_ostream<char_t, traits>& stream, const hash_algorithm value)
    {
        switch (value)
        {
        case hash_algorithm::none:
            stream << "none";
            break;
        case hash_algorithm::md5:
            stream << "md5";
            break;
        case hash_algorithm::sha1:
            stream << "sha1";
            break;
        case hash_algorithm::sha256:
            stream << "sha256";
            break;
        case hash_algorithm::sha384:
            stream << "sha384";
            break;
        case hash_algorithm::sha512:
            stream << "sha512";
            break;
        default:
            stream << static_cast<int>(value);
            break;
        }

        return stream;
    }
}}}
