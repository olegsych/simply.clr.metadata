#include <memory>
#include <sstream>
#include <stdexcept>
#include "interop.h"
#include "builtin_type_signature.h"
#include "custom_type_signature.h"
#include "signature.h"

using namespace std;

namespace simply { namespace clr { namespace metadata { namespace implementation
{
    signature::signature(const void* begin, const void* end)
		: current { reinterpret_cast<const uint8_t*>(begin) }, end { reinterpret_cast<const uint8_t*>(end) }
	{
		if (!begin)
		{
			throw invalid_argument { "begin must not be a nullptr." };
		}

		if (!end)
		{
			throw invalid_argument { "end must not be a nullptr." };
		}

		if (end < begin)
		{
			throw invalid_argument { "begin must point before end." };
		}
	}

	uint8_t signature::read_byte()
	{
		if (current < end)
		{
			return *current++;
		}

		throw out_of_range { "attempting to read past end of signature." };
	}

    uint32_t signature::read_unsigned_integer()
    {
        uint8_t first = read_byte();

        if ((first & 0xC0) == 0xC0)
        {
            uint8_t second = read_byte();
            uint8_t third = read_byte();
            uint8_t fourth = read_byte();
            return ((first & 0x3F) << 24) + (second << 16) + (third << 8) + fourth;
        }

        if ((first & 0x80) == 0x80)
        {
            uint8_t second = read_byte();
            return ((first & 0x7F) << 8) + second;
        }

        return first;
    }

    token signature::read_type_token()
    {
        uint32_t encoded { read_unsigned_integer() };
        uint32_t token_type { encoded & 0x03 };
        uint32_t token_value { encoded >> 2 };
        switch (token_type)
        {
            case 0: return token { table::type, token_value };
            case 1: return token { table::type_reference, token_value };
            case 2: return token { table::type_specification, token_value };
            default: throw logic_error { "Unexpected token type: 3." };
        }
    }

    unique_ptr<type_signature> make_builtin_type_signature(builtin_type builtin_type)
    {
        return unique_ptr<type_signature> { new builtin_type_signature { builtin_type } };
    }

    unique_ptr<type_signature> signature::read_type_signature()
    {
        CorElementType element_type = static_cast<CorElementType>(read_byte());
        switch (element_type)
        {
            case ELEMENT_TYPE_BOOLEAN: return make_builtin_type_signature(builtin_type::Boolean);
            case ELEMENT_TYPE_CHAR:    return make_builtin_type_signature(builtin_type::Char);
            case ELEMENT_TYPE_I:       return make_builtin_type_signature(builtin_type::IntPtr);
            case ELEMENT_TYPE_U:       return make_builtin_type_signature(builtin_type::UIntPtr);
            case ELEMENT_TYPE_I1:      return make_builtin_type_signature(builtin_type::SByte);
            case ELEMENT_TYPE_U1:      return make_builtin_type_signature(builtin_type::Byte);
            case ELEMENT_TYPE_I2:      return make_builtin_type_signature(builtin_type::Int16);
            case ELEMENT_TYPE_U2:      return make_builtin_type_signature(builtin_type::UInt16);
            case ELEMENT_TYPE_I4:      return make_builtin_type_signature(builtin_type::Int32);
            case ELEMENT_TYPE_U4:      return make_builtin_type_signature(builtin_type::UInt32);
            case ELEMENT_TYPE_I8:      return make_builtin_type_signature(builtin_type::Int64);
            case ELEMENT_TYPE_U8:      return make_builtin_type_signature(builtin_type::UInt64);
            case ELEMENT_TYPE_R4:      return make_builtin_type_signature(builtin_type::Single);
            case ELEMENT_TYPE_R8:      return make_builtin_type_signature(builtin_type::Double);
            case ELEMENT_TYPE_STRING:  return make_builtin_type_signature(builtin_type::String);
            case ELEMENT_TYPE_OBJECT:  return make_builtin_type_signature(builtin_type::Object);

            case ELEMENT_TYPE_CLASS:
            case ELEMENT_TYPE_VALUETYPE:
                return unique_ptr<type_signature> { new custom_type_signature { read_type_token() } };

            default:
                ostringstream message;
                message << "Unexpected element type: 0x" << hex << element_type;
                throw logic_error { message.str() };
        }
    }
}}}}
