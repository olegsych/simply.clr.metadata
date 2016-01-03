#include <CppUnitTest.h>
#include <simply/assert.h>
#include <simply/clr/metadata/implementation/builtin_type.h>

using namespace std;
using namespace simply;

namespace simply { namespace clr { namespace metadata { namespace implementation
{
    TEST_CLASS(system_type_signature_test)
    {
        void stream_insertion_operator_writes_expected_string_to_stream(const string& expected, builtin_type value)
        {
            ostringstream stream;
            stream << value;
            assert::is_equal(expected, stream.str());
        }
    public:
        TEST_METHOD(stream_insertion_operator_writes_string_representation_of_given_system_type_to_stream)
        {
            stream_insertion_operator_writes_expected_string_to_stream("Boolean", builtin_type::Boolean);
            stream_insertion_operator_writes_expected_string_to_stream("Char", builtin_type::Char);
            stream_insertion_operator_writes_expected_string_to_stream("SByte", builtin_type::SByte);
            stream_insertion_operator_writes_expected_string_to_stream("Byte", builtin_type::Byte);
            stream_insertion_operator_writes_expected_string_to_stream("Int16", builtin_type::Int16);
            stream_insertion_operator_writes_expected_string_to_stream("UInt16", builtin_type::UInt16);
            stream_insertion_operator_writes_expected_string_to_stream("Int32", builtin_type::Int32);
            stream_insertion_operator_writes_expected_string_to_stream("UInt32", builtin_type::UInt32);
            stream_insertion_operator_writes_expected_string_to_stream("Int64", builtin_type::Int64);
            stream_insertion_operator_writes_expected_string_to_stream("UInt64", builtin_type::UInt64);
            stream_insertion_operator_writes_expected_string_to_stream("Single", builtin_type::Single);
            stream_insertion_operator_writes_expected_string_to_stream("Double", builtin_type::Double);
            stream_insertion_operator_writes_expected_string_to_stream("IntPtr", builtin_type::IntPtr);
            stream_insertion_operator_writes_expected_string_to_stream("UIntPtr", builtin_type::UIntPtr);
            stream_insertion_operator_writes_expected_string_to_stream("Object", builtin_type::Object);
            stream_insertion_operator_writes_expected_string_to_stream("String", builtin_type::String);
            stream_insertion_operator_writes_expected_string_to_stream("42", static_cast<builtin_type>(42));
        }
    };
}}}}
