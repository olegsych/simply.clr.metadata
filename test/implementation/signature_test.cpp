#include "stdafx.h"
#include <CppUnitTest.h>
#include <cstdint>
#include <simply/assert.h>
#include <simply/clr/metadata/implementation/signature.h>

using namespace std;

namespace simply { namespace clr { namespace metadata { namespace implementation
{
	TEST_CLASS(signature_test)
	{
        uint8_t any_blob[1] { 0x00 };
        signature any_signature { begin(any_blob), end(any_blob) };
	public:
		TEST_METHOD(constructor_throws_ivalid_argument_when_begin_pointer_is_nullptr)
		{
			uint8_t blob[42];
			auto e = assert::throws<invalid_argument>([&] { signature { nullptr, end(blob) }; });
			assert::is_equal("begin must not be a nullptr.", e->what());
		}

		TEST_METHOD(constructor_throws_invalid_argument_when_end_pointer_is_nullptr)
		{
			uint8_t blob[42];
			auto e = assert::throws<invalid_argument>([&] { signature { begin(blob), nullptr }; });
			assert::is_equal("end must not be a nullptr.", e->what());
		}

		TEST_METHOD(constructor_throws_invalid_argument_when_begin_points_after_end)
		{
			uint8_t blob[42];
			auto e = assert::throws<invalid_argument>([&] { signature { end(blob), begin(blob) }; });
			assert::is_equal("begin must point before end.", e->what());
		}

		TEST_METHOD(read_byte_returns_value_at_beginning_of_signature)
		{
			const uint8_t expected { 0x42 };
			uint8_t blob[] { expected };
			signature signature { begin(blob), end(blob) };
            const uint8_t actual = read_byte::value(signature);
			assert::is_equal(expected, actual);
		}

		TEST_METHOD(read_byte_advances_to_next_byte)
		{
			const uint8_t expected { 0x42 };
			uint8_t blob[] { expected - 1, expected };
			signature signature { begin(blob), end(blob) };
            read_byte::value(signature);
            const uint8_t actual = read_byte::value(signature);
			assert::is_equal(expected, actual);
		}

		TEST_METHOD(read_byte_throws_out_of_range_when_end_of_signature_has_already_been_reached)
		{
			uint8_t blob[] { 0x42 };
			signature signature { begin(blob), end(blob) };
            read_byte::value(signature);
            auto e = assert::throws<out_of_range>([&] { read_byte::value(signature); });
			assert::is_equal("attempting to read past end of signature.", e->what());
		}

		TEST_METHOD(read_unsigned_integer_returns_x7F_stored_in_single_byte)
		{
            const uint32_t expected { 0x7F };
            uint8_t blob[] { expected };
            signature signature { begin(blob), end(blob) };
            const uint32_t actual = read_unsigned_integer<>::value(signature);
            assert::is_equal(expected, actual);
		}

		TEST_METHOD(read_unsigned_integer_returns_0x80_stored_in_two_bytes)
		{
            const uint32_t expected { 0x80 };
            uint8_t blob[] { 0x80, expected };
            signature signature { begin(blob), end(blob) };
            const uint32_t actual = read_unsigned_integer<>::value(signature);
            assert::is_equal(expected, actual);
		}

		TEST_METHOD(read_unsigned_integer_returns_0x81_stored_in_two_bytes)
		{
			const uint32_t expected { 0x81 };
			uint8_t blob[] { 0x80, expected };
			signature signature { begin(blob), end(blob) };
			const uint32_t actual = read_unsigned_integer<>::value(signature);
			assert::is_equal(expected, actual);
		}

		TEST_METHOD(read_unsigned_integer_returns_0x3FFF_stored_in_two_bytes)
		{
			const uint32_t expected { 0x3FFF };
			uint8_t blob[] { 0xBF, 0xFF };
			signature signature { begin(blob), end(blob) };
			const uint32_t actual = read_unsigned_integer<>::value(signature);
			assert::is_equal(expected, actual);
		}

		TEST_METHOD(read_unsigned_integer_returns_0x4000_stored_in_four_bytes)
		{
			const uint32_t expected { 0x4000 };
			uint8_t blob[] { 0xC0, 0x00, 0x40, 0x00 };
			signature signature { begin(blob), end(blob) };
			const uint32_t actual = read_unsigned_integer<>::value(signature);
			assert::is_equal(expected, actual);
		}

		TEST_METHOD(read_unsigned_integer_returns_0x4001_stored_in_four_bytes)
		{
			const uint32_t expected { 0x4001 };
			uint8_t blob[] { 0xC0, 0x00, 0x40, 0x01 };
			signature signature { begin(blob), end(blob) };
			const uint32_t actual = read_unsigned_integer<>::value(signature);
			assert::is_equal(expected, actual);
		}

		TEST_METHOD(read_unsigned_integer_returns_0x1FFFFFFF_stored_in_four_bytes)
		{
			const uint32_t expected { 0x1FFFFFFF };
			uint8_t blob[] { 0xDF, 0xFF, 0xFF, 0xFF };
			signature signature { begin(blob), end(blob) };
			const uint32_t actual = read_unsigned_integer<>::value(signature);
			assert::is_equal(expected, actual);
		}

        TEST_METHOD(read_type_token_returns_TypeDef_token_extracted_from_unsigned_integer)
        {
            struct read_unsigned_integer
            {
                static uint32_t value(signature&) { return (0x42 << 2) | 0x00; }
            };
            const uint32_t expected = CorTokenType::mdtTypeDef | 0x42;
            const uint32_t actual = read_type_token<read_unsigned_integer>::value(any_signature);
            assert::is_equal(expected, actual);
        }

        TEST_METHOD(read_type_token_returns_TypeRef_token_extracted_from_unsigned_integer)
        {
            struct read_unsigned_integer
            {
                static uint32_t value(signature&) { return (0x42 << 2) | 0x01; }
            };
            const uint32_t expected { CorTokenType::mdtTypeRef | 0x42 };
            const uint32_t actual = read_type_token<read_unsigned_integer>::value(any_signature);
            assert::is_equal(expected, actual);
        }

        TEST_METHOD(read_type_token_returns_TypeSpec_token_extracted_from_unsigned_integer)
        {
            struct read_unsigned_integer
            {
                static uint32_t value(signature&) { return (0x42 << 2) | 0x02; };
            };
            const uint32_t expected { CorTokenType::mdtTypeSpec | 0x42 };
            const uint32_t actual = read_type_token<read_unsigned_integer>::value(any_signature);
            assert::is_equal(expected, actual);
        }

        TEST_METHOD(read_token_type_throws_logic_error_when_token_type_is_not_recognized_to_fail_fast)
        {
            struct read_unsigned_integer
            {
                static uint32_t value(signature&) { return (0x42 << 2) | 0x03; };
            };
            auto e = assert::throws<logic_error>([&] { read_type_token<read_unsigned_integer>::value(any_signature); });
            assert::find("Unexpected token type", e->what());
        }
	};
}}}}
