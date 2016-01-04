#include <CppUnitTest.h>
#include <cstdint>
#include <simply/assert.h>
#include <simply/clr/metadata/table.h>
#include <simply/clr/metadata/implementation/interop.h>
#include <sstream>
#include <type_traits>

using namespace std;

namespace simply { namespace clr { namespace metadata
{
    TEST_CLASS(table_test)
    {
        void verify_element_value(table element, CorTokenType interop_type)
        {
            uint8_t expected_value { static_cast<uint8_t>(interop_type >> 24) };
            uint8_t actual_value { static_cast<uint8_t>(element) };
            assert::is_equal(expected_value, actual_value);
        }

        void verify_element_name(table element, const string& name)
        {
            ostringstream stream;
            stream << element;
            assert::is_equal(name, stream.str());
            assert::is_equal(name, to_string(element));
        }

    public:
        TEST_METHOD(table_is_eight_bit_enum_representing_first_byte_of_metadata_tokens)
        {
            assert::is_true(is_enum<table>());
            assert::is_same<uint8_t, underlying_type<table>::type>();
        }

        TEST_METHOD(table_enum_defines_elements_for_CorTokenType)
        {
            verify_element_value(table::module, CorTokenType::mdtModule);
            verify_element_value(table::type_reference, CorTokenType::mdtTypeRef);
            verify_element_value(table::type, CorTokenType::mdtTypeDef);
            verify_element_value(table::field, CorTokenType::mdtFieldDef);
            verify_element_value(table::method, CorTokenType::mdtMethodDef);
            verify_element_value(table::parameter, CorTokenType::mdtParamDef);
            verify_element_value(table::interface_implementation, CorTokenType::mdtInterfaceImpl);
            verify_element_value(table::member_reference, CorTokenType::mdtMemberRef);
            verify_element_value(table::attribute, CorTokenType::mdtCustomAttribute);
            verify_element_value(table::permission, CorTokenType::mdtPermission);
            verify_element_value(table::signature, CorTokenType::mdtSignature);
            verify_element_value(table::event, CorTokenType::mdtEvent);
            verify_element_value(table::property, CorTokenType::mdtProperty);
            verify_element_value(table::method_implementation, CorTokenType::mdtMethodImpl);
            verify_element_value(table::module_reference, CorTokenType::mdtModuleRef);
            verify_element_value(table::type_specification, CorTokenType::mdtTypeSpec);
            verify_element_value(table::assembly, CorTokenType::mdtAssembly);
            verify_element_value(table::assembly_reference, CorTokenType::mdtAssemblyRef);
            verify_element_value(table::file, CorTokenType::mdtFile);
            verify_element_value(table::exported_type, CorTokenType::mdtExportedType);
            verify_element_value(table::manifest_resource, CorTokenType::mdtManifestResource);
            verify_element_value(table::generic_parameter, CorTokenType::mdtGenericParam);
            verify_element_value(table::method_specification, CorTokenType::mdtMethodSpec);
            verify_element_value(table::generic_parameter_constraint, CorTokenType::mdtGenericParamConstraint);
            verify_element_value(table::string, CorTokenType::mdtString);
        }

        TEST_METHOD(to_string_and_stream_insert_produce_expected_element_names)
        {
            verify_element_name(table::module, "module");
            verify_element_name(table::type_reference, "type_reference");
            verify_element_name(table::type, "type");
            verify_element_name(table::field, "field");
            verify_element_name(table::method, "method");
            verify_element_name(table::parameter, "parameter");
            verify_element_name(table::interface_implementation, "interface_implementation");
            verify_element_name(table::member_reference, "member_reference");
            verify_element_name(table::attribute, "attribute");
            verify_element_name(table::permission, "permission");
            verify_element_name(table::signature, "signature");
            verify_element_name(table::event, "event");
            verify_element_name(table::property, "property");
            verify_element_name(table::method_implementation, "method_implementation");
            verify_element_name(table::module_reference, "module_reference");
            verify_element_name(table::type_specification, "type_specification");
            verify_element_name(table::assembly, "assembly");
            verify_element_name(table::assembly_reference, "assembly_reference");
            verify_element_name(table::file, "file");
            verify_element_name(table::exported_type, "exported_type");
            verify_element_name(table::manifest_resource, "manifest_resource");
            verify_element_name(table::generic_parameter, "generic_parameter");
            verify_element_name(table::method_specification, "method_specification");
            verify_element_name(table::generic_parameter_constraint, "generic_parameter_constraint");
            verify_element_name(table::string, "string");
            verify_element_name(static_cast<table>(0xFF), "0xFF");
        }
    };
}}}
