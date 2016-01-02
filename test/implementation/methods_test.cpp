#include <CppUnitTest.h>
#include <simply/assert.h>
#include <simply/com.h>
#include <simply/clr/metadata/implementation/methods.h>
#include <simply/clr/metadata/implementation/metadata_enumerable.h>
#include <simply/clr/metadata/implementation/metadata_enumerator.h>
#include <stdexcept>
#include "../stub_metadata.h"

using namespace std;

namespace simply { namespace clr { namespace metadata { namespace implementation
{
	TEST_CLASS(methods_test)
	{
		union element
		{
			method method;
			element() {}
			~element() {}
		};
	public:
		TEST_METHOD(class_inherits_pointer_management_from_metadata_enumerable)
		{
			assert::is_base_of<metadata_enumerable<method>, methods>();
		}

		TEST_METHOD(create_enumerator_returns_type_derived_from_metadata_enumerator_to_guarantee_resource_cleanup)
		{
			stub_metadata metadata;
			methods sut { 0, com_ptr<IMetaDataImport2> { &metadata } };
			unique_ptr<enumerator<method>> result = sut.create_enumerator();
			assert::is_not_null(dynamic_cast<metadata_enumerator<method>*>(result.get()));
		}

		TEST_METHOD(get_next_throws_com_error_when_EnumMethods_returns_error_code)
		{
			stub_metadata metadata;
			metadata.enum_methods = [](HCORENUM*, mdTypeDef, mdMethodDef*, ULONG, ULONG*) { return E_INVALIDARG; };
			methods sut { 0, com_ptr<IMetaDataImport2> { &metadata } };
			unique_ptr<enumerator<method>> enumerator = sut.create_enumerator();
			element result;
			assert::throws<com_error>([&] { enumerator->get_next(&result.method); });
		}

		TEST_METHOD(get_next_returns_true_and_first_element_when_called_first_time)
		{
			mdTypeDef expected_type_token { 420 };
			mdMethodDef expected_method_token { 42 };
			stub_metadata metadata;
			metadata.enum_methods = [&](HCORENUM* enum_handle, mdTypeDef type_token, mdMethodDef* method_tokens, ULONG max_count, ULONG* actual_count)
			{
				if (*enum_handle == nullptr && type_token == expected_type_token && max_count > 0)
				{
					*method_tokens = expected_method_token;
					*actual_count = 1;
					return S_OK;
				}
				return S_FALSE;
			};

			methods sut { expected_type_token, com_ptr<IMetaDataImport2> { &metadata } };
			unique_ptr<enumerator<method>> enumerator { sut.create_enumerator() };

			element element;
			bool result = enumerator->get_next(&element.method);

			assert::is_true(result);
			assert::is_equal(expected_method_token, element.method.token());
		}

		TEST_METHOD(get_next_returns_true_and_second_element_when_called_second_time)
		{
			mdTypeDef expected_type_token { 420 };
			mdMethodDef expected_method_token { 42 };
			auto expected_enum_handle = reinterpret_cast<HCORENUM>(0x42424242);
			stub_metadata metadata;
			metadata.enum_methods = [&](HCORENUM* enum_handle, mdTypeDef type_token, mdMethodDef* method_tokens, ULONG max_count, ULONG* actual_count)
			{
				if (type_token == expected_type_token && max_count > 0)
				{
					if (*enum_handle == nullptr)
					{
						*enum_handle = expected_enum_handle;
						*method_tokens = expected_method_token - 1;
						*actual_count = 1;
						return S_OK;
					}

					if (*enum_handle == expected_enum_handle)
					{
						*method_tokens = expected_method_token;
						*actual_count = 1;
						return S_OK;
					}
				}

				*actual_count = 0;
				return S_FALSE;
			};
			methods sut { expected_type_token, com_ptr<IMetaDataImport2> { &metadata } };
			unique_ptr<enumerator<method>> enumerator = sut.create_enumerator();

			element e;
			assert::is_true(enumerator->get_next(&e.method));
			assert::is_true(enumerator->get_next(&e.method));
			assert::is_equal(expected_method_token, e.method.token());
		}

		TEST_METHOD(get_next_returns_false_and_no_element_when_enum_is_empty)
		{
			mdMethodDef unexpected_method_token { 42 };
			stub_metadata metadata;
			metadata.enum_methods = [&](HCORENUM*, mdTypeDef, mdMethodDef* method_tokens, ULONG, ULONG* actual_count)
			{
				*method_tokens = unexpected_method_token;
				*actual_count = 0;
				return S_FALSE;
			};
			methods sut { 0, com_ptr<IMetaDataImport2> { &metadata } };
			unique_ptr<enumerator<method>> enumerator = sut.create_enumerator();

			element e;
			assert::is_false(enumerator->get_next(&e.method));
			assert::is_not_equal(unexpected_method_token, e.method.token());
		}
	};
}}}}
