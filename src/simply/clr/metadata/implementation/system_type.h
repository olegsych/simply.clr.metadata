#pragma once

#include <ostream>

namespace simply { namespace clr { namespace metadata { namespace implementation
{
    enum class system_type
    {
        Boolean,
        Char,
        SByte,
        Byte,
        Int16,
        UInt16,
        Int32,
        UInt32,
        Int64,
        UInt64,
        Single,
        Double,
        IntPtr,
        UIntPtr,
        Object,
        String
    };

    template <class char_t, class traits>
    std::basic_ostream<char_t, traits>& operator<<(std::basic_ostream<char_t, traits>& stream, const system_type value)
    {
        switch (value)
        {
            case system_type::Boolean:
                stream << "Boolean";
                break;
            case system_type::Char:
                stream << "Char";
                break;
            case system_type::SByte:
                stream << "SByte";
                break;
            case system_type::Byte:
                stream << "Byte";
                break;
            case system_type::Int16:
                stream << "Int16";
                break;
            case system_type::UInt16:
                stream << "UInt16";
                break;
            case system_type::Int32:
                stream << "Int32";
                break;
            case system_type::UInt32:
                stream << "UInt32";
                break;
            case system_type::Int64:
                stream << "Int64";
                break;
            case system_type::UInt64:
                stream << "UInt64";
                break;
            case system_type::Single:
                stream << "Single";
                break;
            case system_type::Double:
                stream << "Double";
                break;
            case system_type::IntPtr:
                stream << "IntPtr";
                break;
            case system_type::UIntPtr:
                stream << "UIntPtr";
                break;
            case system_type::Object:
                stream << "Object";
                break;
            case system_type::String:
                stream << "String";
                break;
            default:
                stream << static_cast<int>(value);
                break;
        }

        return stream;
    }
}}}}
