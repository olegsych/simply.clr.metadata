#include <CppUnitTest.h>
#include <cstdint>
#include <iterator>
#include <simply/assert.h>
#include <simply/clr/metadata/implementation/class_type_signature.h>
#include <simply/clr/metadata/implementation/interop.h>

using namespace simply;
using namespace std;

namespace simply { namespace clr { namespace metadata { namespace implementation
{
    TEST_CLASS(class_type_signature_test)
    {
    public:
        TEST_METHOD(class_inherits_from_type_signature)
        {
            assert::is_base_of<type_signature, class_type_signature>();
        }

        TEST_METHOD(type_token_returns_type_token_parsed_from_blob)
        {
            const unsigned type_index { 5 };
            uint8_t blob[] { (type_index << 2) | 0x01 };
            signature signature { begin(blob), end(blob) };
            class_type_signature sut { signature };
            uint32_t expected { CorTokenType::mdtTypeRef | type_index };
            assert::is_equal(expected, sut.type_token());
        }
    };
}}}}
