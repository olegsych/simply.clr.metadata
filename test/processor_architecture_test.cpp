#include <CppUnitTest.h>
#include <simply/assert.h>
#include <simply/clr/metadata/processor_architecture.h>

using namespace std;

namespace simply { namespace clr { namespace metadata
{
    TEST_CLASS(processor_architecture_test)
    {
        void stream_insertion_operator_writes_expected_string_to_stream(const string& expected, processor_architecture value)
        {
            ostringstream stream;

            stream << value;

            assert::is_equal(expected, stream.str());
        }

    public:
        TEST_METHOD(stream_insertion_operator_writes_string_representation_of_given_value_to_stream)
        {
            stream_insertion_operator_writes_expected_string_to_stream("none", processor_architecture::none);
            stream_insertion_operator_writes_expected_string_to_stream("msil", processor_architecture::msil);
            stream_insertion_operator_writes_expected_string_to_stream("x86", processor_architecture::x86);
            stream_insertion_operator_writes_expected_string_to_stream("ia64", processor_architecture::ia64);
            stream_insertion_operator_writes_expected_string_to_stream("amd64", processor_architecture::amd64);
            stream_insertion_operator_writes_expected_string_to_stream("arm", processor_architecture::arm);
            stream_insertion_operator_writes_expected_string_to_stream("42", static_cast<processor_architecture>(42));
        }
    };
}}}
