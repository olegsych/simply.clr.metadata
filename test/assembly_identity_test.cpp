#include <CppUnitTest.h>
#include <simply/assert.h>
#include <simply/clr/metadata/assembly_identity.h>

using namespace simply;
using namespace std;

namespace simply { namespace clr { namespace metadata
{
    TEST_CLASS(assembly_identity_test)
    {
    public:
        TEST_METHOD(constructor_initializes_properties_of_the_instance_with_given_arguments)
        {
            const wstring& expected_name { L"TestAssembly" };
            const hash_algorithm expected_hash { hash_algorithm::sha384 };
            const processor_architecture expected_architecture { processor_architecture::amd64 };
            const version expected_version { 1, 2, 3, 4 };
            const vector<unsigned char>& expected_key { 0x41, 0x42, 0x43, 0x44 };

            assembly_identity sut { expected_name, expected_hash, expected_architecture, expected_version, expected_key };

            assert::is_equal(expected_name, sut.name());
            assert::is_equal(expected_hash, sut.hash_algorithm());
            assert::is_equal(expected_architecture, sut.processor_architecture());
            assert::is_equal(expected_version, sut.version());
            assert::is_true(expected_key == sut.public_key()); // because there is no << for vector<unsigned char>
        }

        TEST_METHOD(name_returns_const_wstring_reference_to_avoid_overhead_of_copying)
        {
            assert::is_same<const wstring&, decltype(declval<assembly_identity>().name())>();
        }

        TEST_METHOD(public_key_returns_const_vector_of_unsigned_char_to_avoid_overhead_of_copying)
        {
            assert::is_same<const vector<unsigned char>&, decltype(declval<assembly_identity>().public_key())>();
        }
    };
}}}
