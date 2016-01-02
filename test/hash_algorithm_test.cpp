#include <CppUnitTest.h>
#include <simply/assert.h>
#include <simply/clr/metadata/hash_algorithm.h>

using namespace std;

namespace simply { namespace clr { namespace metadata
{
    TEST_CLASS(hash_algorithm_test)
    {
        void stream_insertion_operator_writes_expected_string_to_stream(const string& expected, hash_algorithm value)
        {
            ostringstream stream;

            stream << value;

            assert::is_equal(expected, stream.str());
        }

    public:
        TEST_METHOD(stream_insertion_operator_writes_string_representation_of_given_value_to_stream)
        {
            stream_insertion_operator_writes_expected_string_to_stream("none",   hash_algorithm::none);
            stream_insertion_operator_writes_expected_string_to_stream("md5",    hash_algorithm::md5);
            stream_insertion_operator_writes_expected_string_to_stream("sha1",   hash_algorithm::sha1);
            stream_insertion_operator_writes_expected_string_to_stream("sha256", hash_algorithm::sha256);
            stream_insertion_operator_writes_expected_string_to_stream("sha384", hash_algorithm::sha384);
            stream_insertion_operator_writes_expected_string_to_stream("sha512", hash_algorithm::sha512);
            stream_insertion_operator_writes_expected_string_to_stream("42", static_cast<hash_algorithm>(42));
        }
    };
}}}
