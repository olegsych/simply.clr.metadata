#include <stdexcept>
#include <CppUnitTest.h>
#include <simply/assert.h>
#include <simply/clr/metadata/method.h>
#include <simply/com.h>
#include <utility>
#include "stub_metadata.h"

using namespace std;

namespace simply { namespace clr { namespace metadata
{
    TEST_CLASS(method_test)
    {
        stub_metadata metadata;
    public:
		TEST_METHOD(constructor_throws_invalid_argument_when_IMetaDataImport2_is_not_specified)
		{
			com_ptr<IMetaDataImport2> invalid;
			auto e = assert::throws<invalid_argument>([&] { method { 0, invalid }; });
			assert::is_equal("metadata must not be a nullptr.", e->what());
		}

		TEST_METHOD(constructors_adds_reference_to_IMetaDataImport2_object_after_storing_it)
		{
			com_ptr<IMetaDataImport2> pointer { &metadata };
			bool reference_added { false };
			metadata.add_ref = [&]
			{
				reference_added = true;
				return 2;
			};

			method sut { 0, move(pointer) };

			assert::is_true(reference_added);
		}

		TEST_METHOD(destructor_releases_IMetaDataImport2_object_stored_by_constructor)
		{
			com_ptr<IMetaDataImport2> pointer { &metadata };
			method sut { 0, move(pointer) };
			bool released { false };
			metadata.release = [&]
			{
				released = true;
				return 1;
			};

			sut.~method();

			assert::is_true(released);
		}

		TEST_METHOD(name_returns_const_wstring_because_it_cannot_be_changed)
		{
			assert::is_same<const wstring, decltype(declval<method>().name())>();
		}

		TEST_METHOD(name_returns_value_supplied_by_IMetaDataImport2_GetMethodProps_and_no_other_information)
		{
			const mdMethodDef expected_token { 42 };
			const wstring expected_name { L"TestMethod" };
			mdMethodDef actual_token;
			metadata.get_method_props = [&](mdMethodDef token, mdTypeDef*, LPWSTR name, ULONG, ULONG* name_length, DWORD*, PCCOR_SIGNATURE*, ULONG*, ULONG*, DWORD*)
			{
				actual_token = token;
				lstrcpy(name, expected_name.c_str());
				*name_length = static_cast<ULONG>(expected_name.length());
				return S_OK;
			};

			const method sut { expected_token, &metadata };
			const wstring actual_name { sut.name() };

			assert::is_equal(expected_token, actual_token);
			assert::is_equal(expected_name, actual_name);
		}

		TEST_METHOD(name_throws_com_error_when_IMetaDataImport2_GetMethodPros_returns_failure_code)
		{
			metadata.get_method_props = [](mdMethodDef, mdTypeDef*, LPWSTR, ULONG, ULONG*, DWORD*, PCCOR_SIGNATURE*, ULONG*, ULONG*, DWORD*)
			{
				return E_INVALIDARG;
			};
			const method sut { 42, &metadata };
			auto e = assert::throws<com_error>([&] { sut.name(); });
			assert::is_equal(E_INVALIDARG, e->hresult());
		}

		TEST_METHOD(token_returns_value_stored_by_constructor)
		{
			mdMethodDef expected { 42 };
			const method sut { expected, &metadata };
			assert::is_equal(expected, sut.token());
		}

		#pragma region operator==()

		TEST_METHOD(methods_are_equal_if_they_have_identical_tokens_and_metadata_scopes)
		{
			mdMethodDef token { 42 };
			stub_metadata metadata;
			const method left { token, &metadata };
			const method right { token, &metadata };
			assert::is_true(left == right);
		}

		TEST_METHOD(methods_are_not_equal_if_they_have_different_tokens_in_same_metadata_scope)
		{
			stub_metadata metadata;
			const method left { 42, &metadata };
			const method right { 24, &metadata };
			assert::is_false(left == right);
		}

		TEST_METHOD(types_are_not_equal_of_they_have_same_token_in_different_metadata_scopes)
		{
			mdMethodDef token { 42 };
			stub_metadata left_metadata;
			const method left { token, &left_metadata };
			stub_metadata right_metadata;
			const method right { token, &right_metadata };
			assert::is_false(left == right);
		}

		#pragma endregion
	};
}}}
