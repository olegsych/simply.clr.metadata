#include <algorithm>
#include <CppUnitTest.h>
#include <simply/assert.h>
#include <simply/clr/metadata/type.h>
#include <simply/com.h>
#include "stub_metadata.h"

using namespace std;

namespace simply { namespace clr { namespace metadata
{
    TEST_CLASS(type_test)
    {
        stub_metadata metadata;

    public:
        #pragma region constructor

        TEST_METHOD(constructor_stores_token_in_const_field_because_type_identity_cant_change)
        {
            const metadata::token token { 42 };

            type sut { token, &metadata };

            assert::is_same<const metadata::token, decltype(sut._token)>();
            assert::is_equal(token, sut._token);
        }

        TEST_METHOD(constructor_throws_invalid_argument_when_IMetaDataImport2_is_nullptr_to_fail_fast)
        {
            assert::throws<invalid_argument>([&] { type { token {0}, nullptr }; });
        }

        TEST_METHOD(constructor_stores_IMetaDataImport2_object_in_const_com_ptr_to_ensure_its_released)
        {
            type sut { token {0}, &metadata};
            assert::is_same<const com_ptr<IMetaDataImport2>, decltype(sut._metadata)>();
            assert::is_equal(static_cast<IMetaDataImport2*>(&metadata), sut._metadata.get());
        }

        #pragma endregion

        #pragma region assembly

        TEST_METHOD(assembly_returns_assembly_created_with_IMetaDataAssemblyImport_object_obtained_from_metadata)
        {
            mdAssembly expected { 42 };
            metadata.get_assembly_from_scope = [&](mdAssembly* token)
            {
                *token = expected;
                return S_OK;
            };
            type sut { token { 0 }, &metadata };

            assembly result = sut.assembly();

            assert::is_equal<mdAssembly>(expected, result.token());
        }

        TEST_METHOD(assembly_throws_com_error_if_metadata_QueryInterface_fails)
        {
            metadata.query_interface = [](const GUID&, void**) { return E_NOINTERFACE; };
            type sut { token { 0 }, &metadata };
            auto actual = assert::throws<com_error>([&]{ sut.assembly(); });
            assert::is_equal(E_NOINTERFACE, actual->hresult());
        }

        TEST_METHOD(assembly_releases_IMetaDataAssemblyImport_when_its_no_longer_needed)
        {
            type sut { token { 0 }, &metadata };

            int reference_count = 0;
            metadata.add_ref = [&]()
            {
                ++reference_count;
                return 2;
            };
            metadata.release = [&]()
            {
                --reference_count;
                return 1;
            };
            sut.assembly();

            assert::is_equal(0, reference_count);
        }

        #pragma endregion

        #pragma region methods

        TEST_METHOD(methods_returns_range_of_methods_defined_in_type)
        {
            const token expected_type_token { 420 };
            mdMethodDef expected_method_token { 42 };
            metadata.enum_methods = [&](HCORENUM* enum_handle, mdTypeDef type_token, mdMethodDef* method_tokens, ULONG, ULONG* token_count)
            {
                if (*enum_handle == nullptr && type_token == expected_type_token)
                {
                    *enum_handle = reinterpret_cast<HCORENUM>(-1);
                    *method_tokens = expected_method_token;
                    *token_count = 1;
                    return S_OK;
                }

                return S_FALSE;
            };
            const type sut { expected_type_token, &metadata };

            range<method> actual = sut.methods();

            method expected[] { method { expected_method_token, &metadata } };
            assert::is_true(equal(begin(expected), end(expected), actual.begin()));
            assert::is_equal(1, count_if(actual.begin(), actual.end(), [](method){ return true; }));
        }

        #pragma endregion

        #pragma region name

        TEST_METHOD(name_returns_const_wstring_because_type_name_cannot_be_changed)
        {
            assert::is_same<const wstring, decltype(declval<type>().name())>();
        }

        TEST_METHOD(name_returns_value_provided_by_GetTypeDefProps_of_IMetaDataImport2_for_mdTypeDef_token)
        {
            const token expected_token { 42 };
            const wstring expected_name { L"TestNamespace.TestType" };
            mdTypeDef actual_token;
            ULONG actual_buffer_size;
            metadata.get_type_def_props = [&](mdTypeDef token, LPWSTR name_buffer, ULONG name_buffer_size, ULONG* name_length, DWORD*, mdToken*)
            {
                actual_token = token;
                actual_buffer_size = name_buffer_size;
                lstrcpy(name_buffer, expected_name.c_str());
                *name_length = static_cast<ULONG>(expected_name.length());
                return S_OK;
            };

            type sut { expected_token, &metadata };
            const wstring actual_name = sut.name();

            assert::is_equal<mdTypeDef>(expected_token, actual_token);
            assert::is_equal(expected_name, actual_name);
        }

        TEST_METHOD(name_throws_com_error_if_GetTypeDefProps_of_IMetaDataImport2_returns_failure_code)
        {
            type sut { token { 0 }, &metadata };
            metadata.get_type_def_props = [&](mdTypeDef, LPWSTR, ULONG, ULONG*, DWORD*, mdToken*){ return E_NOTIMPL; };
            assert::throws<com_error>([&] { sut.name(); });
        }

        #pragma endregion

        #pragma region token

        TEST_METHOD(token_returns_value_specified_in_constructor)
        {
            const token expected { 42 };
            type sut { expected, &metadata };

            token actual { sut.token() };

            assert::is_equal(expected, actual);
        }

        #pragma endregion

        #pragma region operator==()

        TEST_METHOD(types_are_equal_if_they_have_identical_tokens_and_metadata_scopes)
        {
            metadata::token token { 42 };
            stub_metadata metadata;
            const type left { token, &metadata };
            const type right { token, &metadata };
            assert::is_true(left == right);
        }

        TEST_METHOD(types_are_not_equal_if_they_have_different_tokens_in_same_metadata_scope)
        {
            stub_metadata metadata;
            const type left { token { 42 }, &metadata };
            const type right { token { 24 }, &metadata };
            assert::is_false(left == right);
        }

        TEST_METHOD(types_are_not_equal_of_they_have_same_token_in_different_metadata_scopes)
        {
            metadata::token token { 42 };
            stub_metadata left_metadata;
            const type left { token, &left_metadata };
            stub_metadata right_metadata;
            const type right { token, &right_metadata };
            assert::is_false(left == right);
        }

        #pragma endregion
    };
}}}
