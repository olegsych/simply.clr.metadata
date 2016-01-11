#include <CppUnitTest.h>
#include <cstdint>
#include <iterator>
#include <simply/assert.h>
#include <simply/clr/metadata/implementation/custom_type_signature.h>
#include <simply/clr/metadata/implementation/interop.h>

using namespace simply;
using namespace std;

namespace simply { namespace clr { namespace metadata { namespace implementation
{
    TEST_CLASS(custom_type_signature_test)
    {
    public:
        TEST_METHOD(class_inherits_from_type_signature)
        {
            assert::is_base_of<type_signature, custom_type_signature>();
        }

        TEST_METHOD(token_returns_token_specified_in_costructor)
        {
            token expected { table::type_reference, 5 };
            custom_type_signature sut { expected };

            const token actual { sut.token() };

            assert::is_equal(expected.table(), actual.table());
            assert::is_equal(expected.index(), actual.index());
        }
    };
}}}}
