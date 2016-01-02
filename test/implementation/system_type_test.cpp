#include <CppUnitTest.h>
#include <simply/assert.h>
#include <simply/clr/metadata/implementation/system_type.h>

using namespace std;
using namespace simply;

namespace simply { namespace clr { namespace metadata { namespace implementation
{
    TEST_CLASS(system_type_signature_test)
    {
        void stream_insertion_operator_writes_expected_string_to_stream(const string& expected, system_type value)
        {
            ostringstream stream;
            stream << value;
            assert::is_equal(expected, stream.str());
        }
    public:
        TEST_METHOD(stream_insertion_operator_writes_string_representation_of_given_system_type_to_stream)
        {
            stream_insertion_operator_writes_expected_string_to_stream("Boolean", system_type::Boolean);
            stream_insertion_operator_writes_expected_string_to_stream("Char", system_type::Char);
            stream_insertion_operator_writes_expected_string_to_stream("SByte", system_type::SByte);
            stream_insertion_operator_writes_expected_string_to_stream("Byte", system_type::Byte);
            stream_insertion_operator_writes_expected_string_to_stream("Int16", system_type::Int16);
            stream_insertion_operator_writes_expected_string_to_stream("UInt16", system_type::UInt16);
            stream_insertion_operator_writes_expected_string_to_stream("Int32", system_type::Int32);
            stream_insertion_operator_writes_expected_string_to_stream("UInt32", system_type::UInt32);
            stream_insertion_operator_writes_expected_string_to_stream("Int64", system_type::Int64);
            stream_insertion_operator_writes_expected_string_to_stream("UInt64", system_type::UInt64);
            stream_insertion_operator_writes_expected_string_to_stream("Single", system_type::Single);
            stream_insertion_operator_writes_expected_string_to_stream("Double", system_type::Double);
            stream_insertion_operator_writes_expected_string_to_stream("IntPtr", system_type::IntPtr);
            stream_insertion_operator_writes_expected_string_to_stream("UIntPtr", system_type::UIntPtr);
            stream_insertion_operator_writes_expected_string_to_stream("Object", system_type::Object);
            stream_insertion_operator_writes_expected_string_to_stream("String", system_type::String);
            stream_insertion_operator_writes_expected_string_to_stream("42", static_cast<system_type>(42));
        }
    };
}}}}
