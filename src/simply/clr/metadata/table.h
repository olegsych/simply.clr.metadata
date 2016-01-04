#pragma once

#include <cstdint>
#include <sstream>

namespace simply { namespace clr { namespace metadata
{
    /// <summary>
    /// A CLR metadata table; used as top byte in metadata tokens.
    /// </summary>
    enum class table : uint8_t
    {
        module                       = 0x00, // CorTokenType::mdtModule
        type_reference               = 0x01, // CorTokenType::mdtTypeRef
        type                         = 0x02, // CorTokenType::mdtTypeDef
        field                        = 0x04, // CorTokenType::mdtFieldDef
        method                       = 0x06, // CorTokenType::mdtMethodDef
        parameter                    = 0x08, // CorTokenType::mdtParamDef
        interface_implementation     = 0x09, // CorTokenType::mdtInterfaceImpl
        member_reference             = 0x0A, // CorTokenType::mdtMemberRef
        attribute                    = 0x0C, // CorTokenType::mdtCustomAttribute
        permission                   = 0x0E, // CorTokenType::mdtPermission
        signature                    = 0x11, // CorTokenType::mdtSignature
        event                        = 0x14, // CorTokenType::mdtEvent
        property                     = 0x17, // CorTokenType::mdtProperty
        method_implementation        = 0x19, // CorTokenType::mdtMethodImpl
        module_reference             = 0x1A, // CorTokenType::mdtModuleRef
        type_specification           = 0x1B, // CorTokenType::mdtTypeSpec
        assembly                     = 0x20, // CorTokenType::mdtAssembly
        assembly_reference           = 0x23, // CorTokenType::mdtAssemblyRef
        file                         = 0x26, // CorTokenType::mdtFile
        exported_type                = 0x27, // CorTokenType::mdtExportedType
        manifest_resource            = 0x28, // CorTokenType::mdtManifestResource
        generic_parameter            = 0x2A, // CorTokenType::mdtGenericParam
        method_specification         = 0x2B, // CorTokenType::mdtMethodSpec
        generic_parameter_constraint = 0x2C, // CorTokenType::mdtGenericParamConstraint
        string                       = 0x70, // CorTokenType::mdtString
                                             // CorTokenType::mdtName = 0x71000000
                                             // CorTokenType::mdtBaseType = 0x72000000
    };

    template <class char_t = std::string::value_type, class traits = std::string::traits_type>
    std::basic_string<char_t, traits> to_string(const table value)
    {
        switch (value)
        {
            case table::module:                       return "module";
            case table::type_reference:               return "type_reference";
            case table::type:                         return "type";
            case table::field:                        return "field";
            case table::method:                       return "method";
            case table::parameter:                    return "parameter";
            case table::interface_implementation:     return "interface_implementation";
            case table::member_reference:             return "member_reference";
            case table::attribute:                    return "attribute";
            case table::permission:                   return "permission";
            case table::signature:                    return "signature";
            case table::event:                        return "event";
            case table::property:                     return "property";
            case table::method_implementation:        return "method_implementation";
            case table::module_reference:             return "module_reference";
            case table::type_specification:           return "type_specification";
            case table::assembly:                     return "assembly";
            case table::assembly_reference:           return "assembly_reference";
            case table::file:                         return "file";
            case table::exported_type:                return "exported_type";
            case table::manifest_resource:            return "manifest_resource";
            case table::generic_parameter:            return "generic_parameter";
            case table::method_specification:         return "method_specification";
            case table::generic_parameter_constraint: return "generic_parameter_constraint";
            case table::string:                       return "string";
            default:
                std::basic_stringstream<char_t, traits> stream;
                stream << "0x" << std::uppercase << std::hex << static_cast<unsigned>(value);
                return stream.str();
        }
    }

    template <class char_t, class traits>
    std::basic_ostream<char_t, traits>& operator<<(std::basic_ostream<char_t, traits>& stream, const table value)
    {
        stream << to_string<char_t, traits>(value);
        return stream;
    };
}}}
