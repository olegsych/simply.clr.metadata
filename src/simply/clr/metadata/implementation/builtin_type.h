#pragma once

#include <ostream>

namespace simply { namespace clr { namespace metadata { namespace implementation
{
    enum class builtin_type
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
    std::basic_ostream<char_t, traits>& operator<<(std::basic_ostream<char_t, traits>& stream, const builtin_type value)
    {
        switch (value)
        {
            case builtin_type::Boolean:
                stream << "Boolean";
                break;
            case builtin_type::Char:
                stream << "Char";
                break;
            case builtin_type::SByte:
                stream << "SByte";
                break;
            case builtin_type::Byte:
                stream << "Byte";
                break;
            case builtin_type::Int16:
                stream << "Int16";
                break;
            case builtin_type::UInt16:
                stream << "UInt16";
                break;
            case builtin_type::Int32:
                stream << "Int32";
                break;
            case builtin_type::UInt32:
                stream << "UInt32";
                break;
            case builtin_type::Int64:
                stream << "Int64";
                break;
            case builtin_type::UInt64:
                stream << "UInt64";
                break;
            case builtin_type::Single:
                stream << "Single";
                break;
            case builtin_type::Double:
                stream << "Double";
                break;
            case builtin_type::IntPtr:
                stream << "IntPtr";
                break;
            case builtin_type::UIntPtr:
                stream << "UIntPtr";
                break;
            case builtin_type::Object:
                stream << "Object";
                break;
            case builtin_type::String:
                stream << "String";
                break;
            default:
                stream << static_cast<int>(value);
                break;
        }

        return stream;
    }
}}}}
