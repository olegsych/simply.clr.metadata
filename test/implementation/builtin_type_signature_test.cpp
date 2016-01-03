#include <CppUnitTest.h>
#include <simply/assert.h>
#include <simply/clr/metadata/implementation/builtin_type_signature.h>

using namespace simply;

namespace simply { namespace clr { namespace metadata { namespace implementation
{
    TEST_CLASS(builtin_type_signature_test)
    {
    public:
        TEST_METHOD(class_inherits_from_type_signature)
        {
            assert::is_base_of<type_signature, builtin_type_signature>();
        }

        TEST_METHOD(system_type_returns_value_specified_in_constructor)
        {
            builtin_type expected { builtin_type::IntPtr };
            builtin_type_signature sut { expected };
            assert::is_equal(expected, sut.builtin_type());
        }
    };
}}}}
