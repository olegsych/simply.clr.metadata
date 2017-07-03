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
        TEST_METHOD(public_constructor_throws_out_of_range_when_index_doesnt_fit_in_three_bytes)
        {
            auto index = random<unsigned>(0x01000000);
            auto e = assert::throws<out_of_range>([&] { token { table{}, index }; });
            assert::find(std::to_string(index), e->what());
        }

        TEST_METHOD(public_constructor_initializes_table_and_index_with_given_values)
        {
            auto expected_table { table::type_specification };
            auto expected_index { random<unsigned>(0, 0xFFFFFF) };

            token sut { expected_table, expected_index };

            assert::is_equal(expected_table, sut.table());
            assert::is_equal(expected_index, sut.index());
        }

        TEST_METHOD(value_constructor_initializes_table_and_index_with_given_value)
        {
            auto expected_table { table::type_reference };
            auto expected_index { random<unsigned>(0, 0xFFFFFF) };
            unsigned value { static_cast<uint8_t>(expected_table) << 24 | expected_index };

            token sut { value };

            assert::is_equal(expected_table, sut.table());
            assert::is_equal(expected_index, sut.index());
        }

        TEST_METHOD(value_constructor_is_explicit_to_prevent_accidental_conversion)
        {
            assert::is_convertible<uint32_t, token>(false);
        }

        TEST_METHOD(conversion_operator_returns_value)
        {
            const uint32_t expected { random<uint32_t>() };
            const token sut { expected };

            uint32_t actual = sut;

            assert::is_equal(expected, actual);
        }
    };
}}}
