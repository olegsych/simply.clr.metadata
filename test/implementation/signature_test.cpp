#include <CppUnitTest.h>
#include <cstdint>
#include <simply/assert.h>
#include <simply/random.h>
#include <simply/clr/metadata/implementation/builtin_type_signature.h>
#include <simply/clr/metadata/implementation/custom_type_signature.h>
#include <simply/clr/metadata/implementation/interop.h>
#include <simply/clr/metadata/implementation/signature.h>
#include <simply/clr/metadata/implementation/type_signature.h>

using namespace std;

namespace simply { namespace clr { namespace metadata { namespace implementation
{
	TEST_CLASS(signature_test)
	{
        uint8_t any_blob[1] { 0x00 };
        signature any_cursor { begin(any_blob), end(any_blob) };

        void read_type_signature_returns_builtin_type_signature_when_blob_contains_it(CorElementType element_type, builtin_type builtin_type)
        {
            uint8_t blob[] { static_cast<uint8_t>(element_type) };
            signature sut { begin(blob), end(blob) };

            unique_ptr<type_signature> actual { sut.read_type_signature() };

            assert::is_equal(builtin_type, dynamic_cast<builtin_type_signature&>(*actual).builtin_type());
        }

        void read_type_signature_returns_custom_type_signature(CorElementType element, uint8_t token_type, table table)
        {
            const uint8_t type_index { static_cast<uint8_t>(random<uint16_t>(0, 0x1F)) }; // TODO: remove static cast when random supports uint8_t
            uint8_t blob[] {
                static_cast<uint8_t>(element),
                static_cast<uint8_t>((type_index << 2) | token_type)
            };
            signature sut { begin(blob), end(blob) };

            unique_ptr<type_signature> actual { sut.read_type_signature() };

            auto custom_type = dynamic_cast<custom_type_signature&>(*actual);
            assert::is_equal(table, custom_type.token().table());
            assert::is_equal<unsigned>(type_index, custom_type.token().index());
        }

        void read_type_signature_returns_custom_type_signature(CorElementType element)
        {
            read_type_signature_returns_custom_type_signature(element, 0x00, table::type);
            read_type_signature_returns_custom_type_signature(element, 0x01, table::type_reference);
            read_type_signature_returns_custom_type_signature(element, 0x02, table::type_specification);
        }
	public:
        #pragma region constructor

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

        #pragma endregion

        #pragma region read_byte

		TEST_METHOD(read_byte_returns_value_at_beginning_of_signature)
		{
			const uint8_t expected { 0x42 };
			uint8_t blob[] { expected };
            signature sut { begin(blob), end(blob) };
            const uint8_t actual = sut.read_byte();
			assert::is_equal(expected, actual);
		}

		TEST_METHOD(read_byte_advances_to_next_byte)
		{
			const uint8_t expected { 0x42 };
			uint8_t blob[] { expected - 1, expected };
            signature sut { begin(blob), end(blob) };
            sut.read_byte();
            const uint8_t actual = sut.read_byte();
			assert::is_equal(expected, actual);
		}

		TEST_METHOD(read_byte_throws_out_of_range_when_end_of_signature_has_already_been_reached)
		{
			uint8_t blob[] { 0x42 };
            signature sut { begin(blob), end(blob) };
            sut.read_byte();
            auto e = assert::throws<out_of_range>([&] { sut.read_byte(); });
			assert::is_equal("attempting to read past end of signature.", e->what());
		}

        #pragma endregion

        #pragma region read_unsigned_integer

		TEST_METHOD(read_unsigned_integer_returns_x7F_stored_in_single_byte)
		{
            const uint32_t expected { 0x7F };
            uint8_t blob[] { expected };
            signature sut { begin(blob), end(blob) };
            const uint32_t actual = sut.read_unsigned_integer();
            assert::is_equal(expected, actual);
		}

		TEST_METHOD(read_unsigned_integer_returns_0x80_stored_in_two_bytes)
		{
            const uint32_t expected { 0x80 };
            uint8_t blob[] { 0x80, expected };
            signature sut { begin(blob), end(blob) };
            const uint32_t actual = sut.read_unsigned_integer();
            assert::is_equal(expected, actual);
		}

		TEST_METHOD(read_unsigned_integer_returns_0x81_stored_in_two_bytes)
		{
			const uint32_t expected { 0x81 };
			uint8_t blob[] { 0x80, expected };
            signature sut { begin(blob), end(blob) };
			const uint32_t actual = sut.read_unsigned_integer();
			assert::is_equal(expected, actual);
		}

		TEST_METHOD(read_unsigned_integer_returns_0x3FFF_stored_in_two_bytes)
		{
			const uint32_t expected { 0x3FFF };
			uint8_t blob[] { 0xBF, 0xFF };
            signature sut { begin(blob), end(blob) };
			const uint32_t actual = sut.read_unsigned_integer();
			assert::is_equal(expected, actual);
		}

		TEST_METHOD(read_unsigned_integer_returns_0x4000_stored_in_four_bytes)
		{
			const uint32_t expected { 0x4000 };
			uint8_t blob[] { 0xC0, 0x00, 0x40, 0x00 };
            signature sut { begin(blob), end(blob) };
			const uint32_t actual = sut.read_unsigned_integer();
			assert::is_equal(expected, actual);
		}

		TEST_METHOD(read_unsigned_integer_returns_0x4001_stored_in_four_bytes)
		{
			const uint32_t expected { 0x4001 };
			uint8_t blob[] { 0xC0, 0x00, 0x40, 0x01 };
            signature sut { begin(blob), end(blob) };
			const uint32_t actual = sut.read_unsigned_integer();
			assert::is_equal(expected, actual);
		}

		TEST_METHOD(read_unsigned_integer_returns_0x1FFFFFFF_stored_in_four_bytes)
		{
			const uint32_t expected { 0x1FFFFFFF };
			uint8_t blob[] { 0xDF, 0xFF, 0xFF, 0xFF };
            signature sut { begin(blob), end(blob) };
			const uint32_t actual = sut.read_unsigned_integer();
			assert::is_equal(expected, actual);
		}

        #pragma endregion

        #pragma region read_type_token

        TEST_METHOD(read_type_token_returns_TypeDef_token_extracted_from_unsigned_integer)
        {
            const unsigned type_index = 5;
            uint8_t blob[] { (type_index << 2) | 0x00 };
            signature sut { begin(blob), end(blob) };
            const token actual = sut.read_type_token();
            assert::is_equal(table::type, actual.table());
            assert::is_equal(type_index, actual.index());
        }

        TEST_METHOD(read_type_token_returns_TypeRef_token_extracted_from_unsigned_integer)
        {
            const unsigned type_index = 5;
            uint8_t blob[] { (type_index << 2) | 0x01 };
            signature sut { begin(blob), end(blob) };
            const token actual = sut.read_type_token();
            assert::is_equal(table::type_reference, actual.table());
            assert::is_equal(type_index, actual.index());
        }

        TEST_METHOD(read_type_token_returns_TypeSpec_token_extracted_from_unsigned_integer)
        {
            const unsigned type_index = 5;
            uint8_t blob[] { (type_index << 2) | 0x02 };
            signature sut { begin(blob), end(blob) };
            const token actual = sut.read_type_token();
            assert::is_equal(table::type_specification, actual.table());
            assert::is_equal(type_index, actual.index());
        }

        TEST_METHOD(read_token_type_throws_logic_error_when_token_type_is_not_recognized_to_fail_fast)
        {
            const unsigned type_index = 5;
            uint8_t blob[] { (type_index << 2) | 0x03 };
            signature sut { begin(blob), end(blob) };
            auto e = assert::throws<logic_error>([&] { sut.read_type_token(); });
            assert::find("Unexpected token type", e->what());
        }

        #pragma endregion

        #pragma region read_type_signature

        TEST_METHOD(read_type_signature_returns_builtin_type_signature_when_blob_contains_it)
        {
            read_type_signature_returns_builtin_type_signature_when_blob_contains_it(CorElementType::ELEMENT_TYPE_BOOLEAN, builtin_type::Boolean);
            read_type_signature_returns_builtin_type_signature_when_blob_contains_it(CorElementType::ELEMENT_TYPE_CHAR, builtin_type::Char);
            read_type_signature_returns_builtin_type_signature_when_blob_contains_it(CorElementType::ELEMENT_TYPE_I, builtin_type::IntPtr);
            read_type_signature_returns_builtin_type_signature_when_blob_contains_it(CorElementType::ELEMENT_TYPE_U, builtin_type::UIntPtr);
            read_type_signature_returns_builtin_type_signature_when_blob_contains_it(CorElementType::ELEMENT_TYPE_I1, builtin_type::SByte);
            read_type_signature_returns_builtin_type_signature_when_blob_contains_it(CorElementType::ELEMENT_TYPE_U1, builtin_type::Byte);
            read_type_signature_returns_builtin_type_signature_when_blob_contains_it(CorElementType::ELEMENT_TYPE_I2, builtin_type::Int16);
            read_type_signature_returns_builtin_type_signature_when_blob_contains_it(CorElementType::ELEMENT_TYPE_U2, builtin_type::UInt16);
            read_type_signature_returns_builtin_type_signature_when_blob_contains_it(CorElementType::ELEMENT_TYPE_I4, builtin_type::Int32);
            read_type_signature_returns_builtin_type_signature_when_blob_contains_it(CorElementType::ELEMENT_TYPE_U4, builtin_type::UInt32);
            read_type_signature_returns_builtin_type_signature_when_blob_contains_it(CorElementType::ELEMENT_TYPE_I8, builtin_type::Int64);
            read_type_signature_returns_builtin_type_signature_when_blob_contains_it(CorElementType::ELEMENT_TYPE_U8, builtin_type::UInt64);
            read_type_signature_returns_builtin_type_signature_when_blob_contains_it(CorElementType::ELEMENT_TYPE_R4, builtin_type::Single);
            read_type_signature_returns_builtin_type_signature_when_blob_contains_it(CorElementType::ELEMENT_TYPE_R8, builtin_type::Double);
            read_type_signature_returns_builtin_type_signature_when_blob_contains_it(CorElementType::ELEMENT_TYPE_R8, builtin_type::Double);
            read_type_signature_returns_builtin_type_signature_when_blob_contains_it(CorElementType::ELEMENT_TYPE_OBJECT, builtin_type::Object);
            read_type_signature_returns_builtin_type_signature_when_blob_contains_it(CorElementType::ELEMENT_TYPE_STRING, builtin_type::String);
        }

        TEST_METHOD(read_type_signature_returns_custom_type_signature_when_blob_contains_class_or_value_type_token)
        {
            read_type_signature_returns_custom_type_signature(CorElementType::ELEMENT_TYPE_CLASS);
            read_type_signature_returns_custom_type_signature(CorElementType::ELEMENT_TYPE_VALUETYPE);
        }

        TEST_METHOD(read_type_signature_throws_logic_error_when_element_type_is_not_supported)
        {
            uint8_t blob[] { 0xFF };
            signature sut { begin(blob), end(blob) };

            auto e = assert::throws<logic_error>([&] { sut.read_type_signature(); });

            assert::find("Unexpected element type", e->what());
            assert::find("ff", e->what());
        }

        #pragma endregion
	};
}}}}
