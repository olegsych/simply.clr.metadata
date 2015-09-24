#include "stdafx.h"
#include <algorithm>
#include <CppUnitTest.h>
#include <simply/assert.h>
#include <simply/utility.h>
#include <simply/clr/metadata/assembly.h>
#include <simply/clr/metadata/implementation.h>
#include "stub_IMetaDataDispenserEx.h"
#include "stub_metadata.h"

using namespace simply::utility;
using namespace std;
using namespace std::placeholders;

namespace simply { namespace clr { namespace metadata 
{
    TEST_CLASS(assembly_test)
    {
        stub_IMetaDataDispenserEx dispenser;
        stub_metadata metadata;
        temporary<function<com_ptr<IMetaDataDispenserEx>(void)>> create_dispenser_stub
        {
            implementation::create_dispenser,
            [&] { return com_ptr<IMetaDataDispenserEx> { &dispenser }; }
        };

        HRESULT stub_open_scope(const wchar_t*, unsigned long, const GUID& interface_id, IUnknown** object)
        {
            if (interface_id == IID_IMetaDataAssemblyImport)
            {
                *object = static_cast<IMetaDataAssemblyImport*>(&metadata);
				metadata.AddRef();
                return S_OK;
            }
            else if (interface_id == IID_IMetaDataImport2)
            {
                *object = static_cast<IMetaDataImport2*>(&metadata);
				metadata.AddRef();
                return S_OK;
            }

            return E_NOINTERFACE;
        }

    public:
        assembly_test()
        {
            this->dispenser.open_scope = bind(&assembly_test::stub_open_scope, this, _1, _2, _3, _4);
        }

        #pragma region constructor

        TEST_METHOD(constructor_throws_invalid_argument_when_IMetaDataAssemblyImport_is_null)
        {
            assert::throws<invalid_argument>([&] { assembly sut { nullptr }; });
        }

        TEST_METHOD(constructor_stores_IMetaDataAssemblyImport_in_const_com_ptr_to_guarantee_correct_reference_management)
        {
            assembly sut { &this->metadata };
            assert::is_same<const com_ptr<IMetaDataAssemblyImport>, decltype(sut._metadata)>();
            assert::is_equal(static_cast<IMetaDataAssemblyImport*>(&this->metadata), sut._metadata.get());
        }

        #pragma endregion

        #pragma region identity

        TEST_METHOD(identity_invokes_GetAssemblyProps_with_assembly_token_to_obtain_its_properties)
        {
            mdAssembly expected_token { 42 };
			this->metadata.get_assembly_from_scope = [&](mdAssembly* token)
			{
				*token = expected_token;
				return S_OK;
			};
            mdAssembly actual_token;
            this->metadata.get_assembly_props = [&](mdAssembly assembly, const void**, ULONG*, ULONG*, LPWSTR, ULONG, ULONG*, ASSEMBLYMETADATA*, DWORD*)
            {
                actual_token = assembly;
                return E_NOTIMPL;
            };
            assembly sut { &this->metadata };

            auto expected = assert::throws<com_error>([&] { sut.identity(); });

            assert::is_equal(expected_token, actual_token);
			assert::is_equal(E_NOTIMPL, expected->hresult());
		}

        TEST_METHOD(identity_invokes_GetAssemblyProps_with_large_buffer_for_assembly_name)
        {
            wchar_t* actual_buffer;
            unsigned long actual_buffer_size;
            this->metadata.get_assembly_props = [&](mdAssembly, const void**, ULONG*, ULONG*, LPWSTR buffer, ULONG buffer_size, ULONG*, ASSEMBLYMETADATA*, DWORD*)
            {
                actual_buffer = buffer;
                actual_buffer_size = buffer_size;
                return E_NOTIMPL;
            };
            assembly sut { &this->metadata };

            auto expected = assert::throws<com_error>([&] {sut.identity(); });

            assert::is_not_null(actual_buffer);
            assert::is_equal(1024UL, actual_buffer_size);
        }

        TEST_METHOD(identity_returns_assembly_identity_with_name_returned_by_GetAssemblyProps)
        {
            wstring expected_name { L"TestAssembly.dll" };
            this->metadata.get_assembly_props = [&](mdAssembly, const void**, ULONG* key_length, ULONG*, LPWSTR name, ULONG, ULONG* name_length, ASSEMBLYMETADATA*, DWORD*)
            {
                *key_length = 0;
                memcpy(name, expected_name.c_str(), expected_name.length() * sizeof(wchar_t));
                *name_length = static_cast<ULONG>(expected_name.length());
                return S_OK;
            };
            assembly sut { &this->metadata };

            assembly_identity identity = sut.identity();

            assert::is_equal(expected_name, identity.name());
        }

        TEST_METHOD(identity_returns_assembly_identity_with_processor_architecture_returned_by_GetAssemblyProps)
        {
            identity_returns_assembly_identity_with_processor_architecture_returned_by_GetAssemblyProps(processor_architecture::none, CorAssemblyFlags::afPA_None);
            identity_returns_assembly_identity_with_processor_architecture_returned_by_GetAssemblyProps(processor_architecture::msil, CorAssemblyFlags::afPA_MSIL);
            identity_returns_assembly_identity_with_processor_architecture_returned_by_GetAssemblyProps(processor_architecture::x86, CorAssemblyFlags::afPA_x86);
            identity_returns_assembly_identity_with_processor_architecture_returned_by_GetAssemblyProps(processor_architecture::ia64, CorAssemblyFlags::afPA_IA64);
            identity_returns_assembly_identity_with_processor_architecture_returned_by_GetAssemblyProps(processor_architecture::amd64, CorAssemblyFlags::afPA_AMD64);
            identity_returns_assembly_identity_with_processor_architecture_returned_by_GetAssemblyProps(processor_architecture::arm, CorAssemblyFlags::afPA_ARM);
        }

        void identity_returns_assembly_identity_with_processor_architecture_returned_by_GetAssemblyProps(processor_architecture expected, CorAssemblyFlags actual)
        {
            this->metadata.get_assembly_props = [&](mdAssembly, const void**, ULONG* key_length, ULONG*, LPWSTR, ULONG, ULONG* name_length, ASSEMBLYMETADATA*, DWORD* assembly_flags)
            {
                *name_length = 0;
                *key_length = 0;
                *assembly_flags = actual;
                return S_OK;
            };
            assembly sut { &this->metadata };

            assembly_identity identity = sut.identity();

            assert::is_equal(expected, identity.processor_architecture());
        }

        TEST_METHOD(identity_returns_assembly_identity_with_version_returned_by_GetAssemblyProps)
        {
            version expected_version { 1, 2, 3, 4 };
            this->metadata.get_assembly_props = [&](mdAssembly, const void**, ULONG* key_length, ULONG*, LPWSTR, ULONG, ULONG* name_length, ASSEMBLYMETADATA* metadata, DWORD*)
            {
                *name_length = 0;
                *key_length = 0;
                metadata->usMajorVersion = expected_version.major();
                metadata->usMinorVersion = expected_version.minor();
                metadata->usBuildNumber = expected_version.build();
                metadata->usRevisionNumber = expected_version.revision();
                return S_OK;
            };
            assembly sut { &this->metadata };

            assembly_identity identity = sut.identity();

            assert::is_equal(expected_version, identity.version());
        }

        TEST_METHOD(identity_returns_assembly_identity_with_hash_algorithm_returned_by_GetAssemblyProps)
        {
            this->identity_returns_assembly_identity_with_hash_algorithm_returned_by_GetAssemblyProps(hash_algorithm::none, 0);
            this->identity_returns_assembly_identity_with_hash_algorithm_returned_by_GetAssemblyProps(hash_algorithm::md5, 32771);
            this->identity_returns_assembly_identity_with_hash_algorithm_returned_by_GetAssemblyProps(hash_algorithm::sha1, 32772);
            this->identity_returns_assembly_identity_with_hash_algorithm_returned_by_GetAssemblyProps(hash_algorithm::sha256, 32780);
            this->identity_returns_assembly_identity_with_hash_algorithm_returned_by_GetAssemblyProps(hash_algorithm::sha384, 32781);
            this->identity_returns_assembly_identity_with_hash_algorithm_returned_by_GetAssemblyProps(hash_algorithm::sha512, 32782);
        }

        void identity_returns_assembly_identity_with_hash_algorithm_returned_by_GetAssemblyProps(hash_algorithm expected, ULONG actual)
        {
            this->metadata.get_assembly_props = [&](mdAssembly, const void**, ULONG* key_length, ULONG* hash_algorithm, LPWSTR, ULONG, ULONG* name_length, ASSEMBLYMETADATA*, DWORD*)
            {
                *name_length = 0;
                *key_length = 0;
                *hash_algorithm = actual;
                return S_OK;
            };
            assembly sut { &this->metadata };

            assembly_identity identity = sut.identity();

            assert::is_equal(expected, identity.hash_algorithm());
        }

        TEST_METHOD(identity_returns_assembly_identity_with_public_key_returned_by_GetAssemblyProps)
        {
            unsigned char raw_key[] = { 0x41, 0x42, 0x43, 0x44 };
            this->metadata.get_assembly_props = [&](mdAssembly, const void** public_key, ULONG* public_key_length, ULONG*, LPWSTR, ULONG, ULONG* name_length, ASSEMBLYMETADATA*, DWORD*)
            {
                *name_length = 0;
                *public_key = raw_key;
                *public_key_length = sizeof(raw_key);
                return S_OK;
            };
            assembly sut { &this->metadata };

            assembly_identity identity = sut.identity();

            const vector<unsigned char> expected_key { begin(raw_key), end(raw_key) };
            assert::is_true(expected_key == identity.public_key()); // there is no << for vector<unsigned char>
        }

        #pragma endregion

		#pragma region token()

        TEST_METHOD(token_returns_value_returned_by_GetAssemblyFromScope)
        {
            mdAssembly expected { 42 };
			metadata.get_assembly_from_scope = [&](mdAssembly* token)
			{
				*token = expected;
				return S_OK;
			};
			assembly sut { &metadata };
			unsigned int actual = sut.token();
			assert::is_equal(expected, actual);
		}

		TEST_METHOD(token_throws_com_error_when_GetAssemblyFromScope_returns_failure_code)
		{
			metadata.get_assembly_from_scope = [&](mdAssembly* token) { return E_NOTIMPL; };
			assembly sut { &metadata };
			auto actual = assert::throws<com_error>([&] { sut.token(); });
			assert::is_equal(E_NOTIMPL, actual->hresult());
		}

		#pragma endregion

		#pragma region types()

		TEST_METHOD(types_returns_range_of_types_defined_in_assembly)
		{
			const mdTypeDef type_token { 42 };
			stub_metadata metadata;
			metadata.enum_type_defs = [&](HCORENUM* enum_handle, mdTypeDef* token, ULONG, ULONG* count)
			{
				if (*enum_handle == nullptr)
				{
					*enum_handle = reinterpret_cast<HCORENUM>(-1);
					*token = type_token;
					*count = 1;
					return S_OK;
				}
				return S_FALSE;
			};
			assembly sut { &metadata };

			range<type> actual = sut.types();

			type expected[] { type { type_token, &metadata } };			
			assert::is_true(equal(begin(expected), end(expected), actual.begin()));
			assert::is_equal(1, count_if(actual.begin(), actual.end(), [](type) { return true; }));
		}

		TEST_METHOD(types_throws_com_error_when_QueryInterface_does_not_return_IMetaDataImport2_to_fail_fast)
		{
			stub_metadata metadata;
			metadata.query_interface = [](const GUID&, void**) { return E_NOINTERFACE; };
			assembly sut { &metadata };
			auto e = assert::throws<com_error>([&] { sut.types(); });
			assert::is_equal(E_NOINTERFACE, e->hresult());
		}

		#pragma endregion

		#pragma region operator==()

		TEST_METHOD(assemblies_are_equal_when_they_have_same_metadata_scope)
		{
			stub_metadata metadata;
			assembly left { &metadata };
			assembly right { &metadata };
			assert::is_true(left == right);
		}

		TEST_METHOD(assemblies_are_not_equal_when_they_have_different_metadata_scopes)
		{
			stub_metadata left_metadata;
			assembly left { &left_metadata };
			stub_metadata right_metadata;
			assembly right { &right_metadata };
			assert::is_false(left == right);
		}

		#pragma endregion

        #pragma region load_from

        TEST_METHOD(load_from_obtains_IMetaDataAssemblyImport_by_opening_scope_with_metadata_dispenser_in_readonly_mode)
        {
            const wchar_t* requested_scope {};
            unsigned long requested_flags {};
            this->dispenser.open_scope = [&](const wchar_t* scope, unsigned long flags, const GUID& interface_id, IUnknown** object)
            {
                if (interface_id == IID_IMetaDataAssemblyImport)
                {
                    requested_scope = scope;
                    requested_flags = flags;
                }

                return this->stub_open_scope(scope, flags, interface_id, object);
            };

            wstring file_path { L"C:\\path\\assembly.dll" };
            assembly::load_from(file_path);

            assert::is_equal(file_path.c_str(), requested_scope);
            assert::is_equal<unsigned long>(CorOpenFlags::ofReadOnly, requested_flags);
        }

        TEST_METHOD(load_from_checks_HRESULT_returned_by_OpenScope_for_IMetaDataAssemblyImport)
        {
            dispenser.open_scope = [](const wchar_t*, unsigned long, const GUID&, IUnknown**){ return E_INVALIDARG; };
            auto actual = assert::throws<com_error>([] { assembly::load_from(L"any.dll"); });
            assert::is_equal(E_INVALIDARG, actual->hresult());
        }

        #pragma endregion
    };
}}}