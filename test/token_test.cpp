#include <CppUnitTest.h>
#include <simply/assert.h>
#include <simply/clr/metadata/table.h>
#include <simply/clr/metadata/token.h>
#include <simply/random.h>
#include <string>

using namespace std;

namespace simply { namespace clr { namespace metadata
{
    TEST_CLASS(token_test)
    {
    public:
        TEST_METHOD(constructor_throws_out_of_range_when_index_doesnt_fit_in_three_bytes)
        {
            auto index = random<unsigned>(0x01000000);
            auto e = assert::throws<out_of_range>([&] { token { table{}, index }; });
            assert::find(std::to_string(index), e->what());
        }

        TEST_METHOD(table_returns_value_specified_in_constructor)
        {
            table expected { table::type_specification };
            token sut { expected, unsigned{} };
            assert::is_equal(expected, sut.table());
        }

        TEST_METHOD(index_returns_value_specified_in_constructor)
        {
            unsigned expected { random<uint16_t>() };
            token sut { table{}, expected };
            assert::is_equal(expected, sut.index());
        }
    };
}}}
