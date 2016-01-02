#include <CppUnitTest.h>
#include <simply/assert.h>
#include <simply/clr/metadata/implementation.h>

namespace simply { namespace clr { namespace metadata { namespace implementation
{
    TEST_CLASS(implementation_test)
    {
    public:
        TEST_METHOD(create_dispenser_returns_valid_IMetaDataDispenser_pointer)
        {
            com_ptr<IMetaDataDispenserEx> dispenser = create_dispenser();
            com_ptr<IMetaDataDispenserEx> check;
            assert::is_equal(S_OK, dispenser.get()->QueryInterface(IID_IMetaDataDispenserEx, check));
        }
    };
}}}}
