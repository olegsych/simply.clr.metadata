#include "stdafx.h"
#include <CppUnitTest.h>
#include <simply/assert.h>
#include <simply/com.h>
#include <simply/clr/metadata/implementation/types.h>
#include <simply/clr/metadata/implementation/metadata_enumerable.h>
#include <simply/clr/metadata/implementation/metadata_enumerator.h>
#include "../stub_metadata.h"

using namespace std;

namespace simply { namespace clr { namespace metadata { namespace implementation
{
	TEST_CLASS(types_test)
	{
		union element
		{
			char placeholder[sizeof(type)];
			type type;
			element() {}
			~element() {}
		};
	public:
		TEST_METHOD(class_inherits_pointer_management_from_metadeata_enumerable)
		{
			assert::is_base_of<metadata_enumerable<type>, types>();
			assert::is_concrete<types>();
		}

		TEST_METHOD(create_enumerator_returns_type_derived_from_metadata_enumerator_to_guarantee_resource_cleanup)
		{
			stub_metadata metadata;
			types sut { com_ptr<IMetaDataImport2> { &metadata } };
			unique_ptr<enumerator<type>> result = sut.create_enumerator();
			assert::is_not_null(dynamic_cast<metadata_enumerator<type>*>(result.get()));
		}

		TEST_METHOD(get_next_throws_com_error_when_EnumTypeDefs_returns_error_code)
		{
			stub_metadata metadata;
			metadata.enum_type_defs = [](HCORENUM*, mdTypeDef*, ULONG, ULONG*) { return E_INVALIDARG; };
			types sut { com_ptr<IMetaDataImport2> { &metadata } };
			unique_ptr<enumerator<type>> enumerator = sut.create_enumerator();
			element result;
			assert::throws<com_error>([&] { enumerator->get_next(&result.type); });
		}

		TEST_METHOD(get_next_returns_true_and_expected_type_when_called_first_time)
		{
			stub_metadata metadata;
			type expected { 42, &metadata };
			metadata.enum_type_defs = [&](HCORENUM* enum_handle, mdTypeDef* buffer, ULONG buffer_size, ULONG* item_count)
			{
				if (*enum_handle == nullptr)
				{
					*buffer = expected.token();
					*item_count = 1;
					return S_OK;
				}

				return S_FALSE;
			};
			types sut { com_ptr<IMetaDataImport2> { &metadata } };
			unique_ptr<enumerator<type>> enumerator = sut.create_enumerator();

			element actual;
			bool result { enumerator->get_next(&actual.type) };

			assert::is_true(result);
			assert::is_true(expected == actual.type);
		}

		TEST_METHOD(get_next_returns_next_type_when_called_again)
		{
			auto handle = reinterpret_cast<HCORENUM>(0x42);
			stub_metadata metadata;
			metadata.enum_type_defs = [&](HCORENUM* enum_handle, mdTypeDef* buffer, ULONG buffer_size, ULONG* item_count)
			{
				if (*enum_handle == nullptr)
				{
					*enum_handle = handle;
					*buffer = 41;
					*item_count = 1;
					return S_OK;
				}

				if (*enum_handle == handle)
				{
					*buffer = 42;
					*item_count = 1;
					return S_OK;
				}

				*item_count = 0;
				return S_FALSE;
			};
			types sut { com_ptr<IMetaDataImport2> { &metadata } };
			unique_ptr<enumerator<type>> enumerator = sut.create_enumerator();

			element e;
			assert::is_true(enumerator->get_next(&e.type));
			assert::is_equal(41U, e.type.token());

			assert::is_true(enumerator->get_next(&e.type));
			assert::is_equal(42U, e.type.token());
		}

		TEST_METHOD(get_next_returns_false_if_EnumTypeDefs_returns_zero_type_definitions)
		{
			stub_metadata metadata;
			metadata.enum_type_defs = [&](HCORENUM*, mdTypeDef*, ULONG, ULONG* item_count)
			{
				*item_count = 0;
				return S_FALSE;
			};
			types sut { com_ptr<IMetaDataImport2> { &metadata } };
			unique_ptr<enumerator<type>> enumerator = sut.create_enumerator();

			element e;
			bool result = enumerator->get_next(&e.type);

			assert::is_false(result);
		}
	};
}}}}