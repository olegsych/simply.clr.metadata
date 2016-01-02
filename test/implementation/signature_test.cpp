#include <CppUnitTest.h>
#include <cstdint>
#include <simply/assert.h>
#include <simply/clr/metadata/implementation/interop.h>
#include <simply/clr/metadata/implementation/signature.h>

using namespace std;

namespace simply { namespace clr { namespace metadata { namespace implementation
{
	TEST_CLASS(signature_test)
	{
        uint8_t any_blob[1] { 0x00 };
        blob_cursor any_cursor { begin(any_blob), end(any_blob) };
	public:
        #pragma region blob_cursor

		TEST_METHOD(blob_cursor_throws_ivalid_argument_when_begin_pointer_is_nullptr)
		{
			uint8_t blob[42];
			auto e = assert::throws<invalid_argument>([&] { blob_cursor { nullptr, end(blob) }; });
			assert::is_equal("begin must not be a nullptr.", e->what());
		}

		TEST_METHOD(blob_cursor_throws_invalid_argument_when_end_pointer_is_nullptr)
		{
			uint8_t blob[42];
			auto e = assert::throws<invalid_argument>([&] { blob_cursor { begin(blob), nullptr }; });
			assert::is_equal("end must not be a nullptr.", e->what());
		}

		TEST_METHOD(blob_cursor_throws_invalid_argument_when_begin_points_after_end)
		{
			uint8_t blob[42];
			auto e = assert::throws<invalid_argument>([&] { blob_cursor { end(blob), begin(blob) }; });
			assert::is_equal("begin must point before end.", e->what());
		}

        #pragma endregion

        #pragma region read_byte

		TEST_METHOD(read_byte_returns_value_at_beginning_of_signature)
		{
			const uint8_t expected { 0x42 };
			uint8_t blob[] { expected };
            blob_cursor signature { begin(blob), end(blob) };
            const uint8_t actual = read_byte(signature);
			assert::is_equal(expected, actual);
		}

		TEST_METHOD(read_byte_advances_to_next_byte)
		{
			const uint8_t expected { 0x42 };
			uint8_t blob[] { expected - 1, expected };
            blob_cursor signature { begin(blob), end(blob) };
            read_byte(signature);
            const uint8_t actual = read_byte(signature);
			assert::is_equal(expected, actual);
		}

		TEST_METHOD(read_byte_throws_out_of_range_when_end_of_signature_has_already_been_reached)
		{
			uint8_t blob[] { 0x42 };
            blob_cursor signature { begin(blob), end(blob) };
            read_byte(signature);
            auto e = assert::throws<out_of_range>([&] { read_byte(signature); });
			assert::is_equal("attempting to read past end of signature.", e->what());
		}

        #pragma endregion

        #pragma region read_unsigned_integer

		TEST_METHOD(read_unsigned_integer_returns_x7F_stored_in_single_byte)
		{
            const uint32_t expected { 0x7F };
            uint8_t blob[] { expected };
            blob_cursor signature { begin(blob), end(blob) };
            const uint32_t actual = read_unsigned_integer(signature);
            assert::is_equal(expected, actual);
		}

		TEST_METHOD(read_unsigned_integer_returns_0x80_stored_in_two_bytes)
		{
            const uint32_t expected { 0x80 };
            uint8_t blob[] { 0x80, expected };
            blob_cursor signature { begin(blob), end(blob) };
            const uint32_t actual = read_unsigned_integer(signature);
            assert::is_equal(expected, actual);
		}

		TEST_METHOD(read_unsigned_integer_returns_0x81_stored_in_two_bytes)
		{
			const uint32_t expected { 0x81 };
			uint8_t blob[] { 0x80, expected };
            blob_cursor signature { begin(blob), end(blob) };
			const uint32_t actual = read_unsigned_integer(signature);
			assert::is_equal(expected, actual);
		}

		TEST_METHOD(read_unsigned_integer_returns_0x3FFF_stored_in_two_bytes)
		{
			const uint32_t expected { 0x3FFF };
			uint8_t blob[] { 0xBF, 0xFF };
            blob_cursor signature { begin(blob), end(blob) };
			const uint32_t actual = read_unsigned_integer(signature);
			assert::is_equal(expected, actual);
		}

		TEST_METHOD(read_unsigned_integer_returns_0x4000_stored_in_four_bytes)
		{
			const uint32_t expected { 0x4000 };
			uint8_t blob[] { 0xC0, 0x00, 0x40, 0x00 };
            blob_cursor signature { begin(blob), end(blob) };
			const uint32_t actual = read_unsigned_integer(signature);
			assert::is_equal(expected, actual);
		}

		TEST_METHOD(read_unsigned_integer_returns_0x4001_stored_in_four_bytes)
		{
			const uint32_t expected { 0x4001 };
			uint8_t blob[] { 0xC0, 0x00, 0x40, 0x01 };
            blob_cursor signature { begin(blob), end(blob) };
			const uint32_t actual = read_unsigned_integer(signature);
			assert::is_equal(expected, actual);
		}

		TEST_METHOD(read_unsigned_integer_returns_0x1FFFFFFF_stored_in_four_bytes)
		{
			const uint32_t expected { 0x1FFFFFFF };
			uint8_t blob[] { 0xDF, 0xFF, 0xFF, 0xFF };
            blob_cursor signature { begin(blob), end(blob) };
			const uint32_t actual = read_unsigned_integer(signature);
			assert::is_equal(expected, actual);
		}

        #pragma endregion

        TEST_METHOD(read_type_token_returns_TypeDef_token_extracted_from_unsigned_integer)
        {
            const unsigned type_index = 5;
            uint8_t blob[] { (type_index << 2) | 0x00 };
            blob_cursor signature { begin(blob), end(blob) };
            const uint32_t expected = CorTokenType::mdtTypeDef | type_index;
            const uint32_t actual = read_type_token(signature);
            assert::is_equal(expected, actual);
        }

        TEST_METHOD(read_type_token_returns_TypeRef_token_extracted_from_unsigned_integer)
        {
            const unsigned type_index = 5;
            uint8_t blob[] { (type_index << 2) | 0x01 };
            blob_cursor signature { begin(blob), end(blob) };
            const uint32_t expected { CorTokenType::mdtTypeRef | type_index };
            const uint32_t actual = read_type_token(signature);
            assert::is_equal(expected, actual);
        }

        TEST_METHOD(read_type_token_returns_TypeSpec_token_extracted_from_unsigned_integer)
        {
            const unsigned type_index = 5;
            uint8_t blob[] { (type_index << 2) | 0x02 };
            blob_cursor signature { begin(blob), end(blob) };
            const uint32_t expected { CorTokenType::mdtTypeSpec | type_index };
            const uint32_t actual = read_type_token(signature);
            assert::is_equal(expected, actual);
        }

        TEST_METHOD(read_token_type_throws_logic_error_when_token_type_is_not_recognized_to_fail_fast)
        {
            const unsigned type_index = 5;
            uint8_t blob[] { (type_index << 2) | 0x03 };
            blob_cursor signature { begin(blob), end(blob) };
            auto e = assert::throws<logic_error>([&] { read_type_token(signature); });
            assert::find("Unexpected token type", e->what());
        }
	};
}}}}
