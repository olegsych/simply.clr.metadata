#pragma once

#pragma warning(disable: 4091)
#include <cor.h>
#pragma warning(default: 4091)
#include <functional>

using namespace std;

namespace simply { namespace clr { namespace metadata 
{
	/// <summary>
	/// Stub emulating RegMeta implementation in CLR.
	/// https://github.com/dotnet/coreclr/blob/4cf8a6b082d9bb1789facd996d8265d3908757b2/src/md/compiler/regmeta.h
	/// </summary>
    class stub_metadata : public IMetaDataImport2, public IMetaDataAssemblyImport
    {
    public:
		#pragma region IUnknown

		function<ULONG(void)> add_ref = [] { return 2; };
		function<ULONG(void)> release = [] { return 1; };
		function<HRESULT(const GUID&, void**)> query_interface = [&](const GUID& interface_id, void** object) 
		{ 
			if (interface_id == IID_IMetaDataAssemblyImport)
			{
				*object = static_cast<IMetaDataAssemblyImport*>(this);
				AddRef();
				return S_OK;
			}
			else if (interface_id == IID_IMetaDataImport2)
			{
				*object = static_cast<IMetaDataImport2*>(this);
				AddRef();
				return S_OK;
			}

			return E_NOINTERFACE; 
		};

        ULONG __stdcall AddRef(void) override
        {
            return this->add_ref();
        }

        ULONG __stdcall Release(void) override
        {
            return this->release();
        }

		HRESULT __stdcall QueryInterface(const GUID& interface_id, void** object) override
		{
			return this->query_interface(interface_id, object);
		}

		#pragma endregion

		#pragma region IMetaDataImport

		function<void(HCORENUM)> close_enum = [](HCORENUM) {};
		function<HRESULT(HCORENUM*, mdTypeDef*, ULONG, ULONG*)> enum_type_defs = 
			[](HCORENUM*, mdTypeDef*, ULONG, ULONG*) { return S_FALSE; };
		function<HRESULT(mdMethodDef, mdTypeDef*, LPWSTR, ULONG, ULONG*, DWORD*, PCCOR_SIGNATURE*, ULONG*, ULONG*, DWORD*)> get_method_props =
			[](mdMethodDef, mdTypeDef*, LPWSTR, ULONG, ULONG*, DWORD*, PCCOR_SIGNATURE*, ULONG*, ULONG*, DWORD*) { return S_OK; };
		function<HRESULT(mdTypeDef, LPWSTR, ULONG, ULONG*, DWORD*, mdToken*)> get_type_def_props =
			[](mdTypeDef, LPWSTR, ULONG, ULONG*, DWORD*, mdToken*) { return S_OK; };

        void __stdcall IMetaDataImport::CloseEnum(HCORENUM hEnum) override
        {
			this->close_enum(hEnum);
        }

        HRESULT __stdcall CountEnum(HCORENUM hEnum, ULONG * pulCount) override
        {
            return E_NOTIMPL;
        }

        HRESULT __stdcall ResetEnum(HCORENUM hEnum, ULONG ulPos) override
        {
            return E_NOTIMPL;
        }

        HRESULT __stdcall EnumTypeDefs(HCORENUM* enum_handle, mdTypeDef* type_definitions, ULONG max_count, ULONG* actual_count) override
        {
            return enum_type_defs(enum_handle, type_definitions, max_count, actual_count);
        }

        HRESULT __stdcall EnumInterfaceImpls(HCORENUM * phEnum, mdTypeDef td, mdInterfaceImpl rImpls[], ULONG cMax, ULONG * pcImpls) override
        {
            return E_NOTIMPL;
        }

        HRESULT __stdcall EnumTypeRefs(HCORENUM * phEnum, mdTypeRef rTypeRefs[], ULONG cMax, ULONG * pcTypeRefs) override
        {
            return E_NOTIMPL;
        }

        HRESULT __stdcall FindTypeDefByName(LPCWSTR szTypeDef, mdToken tkEnclosingClass, mdTypeDef * ptd) override
        {
            return E_NOTIMPL;
        }

        HRESULT __stdcall GetScopeProps(LPWSTR szName, ULONG cchName, ULONG * pchName, GUID * pmvid) override
        {
            return E_NOTIMPL;
        }

        HRESULT __stdcall GetModuleFromScope(mdModule * pmd) override
        {
            return E_NOTIMPL;
        }

        HRESULT __stdcall GetTypeDefProps(mdTypeDef td, LPWSTR szTypeDef, ULONG cchTypeDef, ULONG* pchTypeDef, DWORD* pdwTypeDefFlags, mdToken* ptkExtends) override
        {
            return this->get_type_def_props(td, szTypeDef, cchTypeDef, pchTypeDef, pdwTypeDefFlags, ptkExtends);
        }

        HRESULT __stdcall GetInterfaceImplProps(mdInterfaceImpl iiImpl, mdTypeDef * pClass, mdToken * ptkIface) override
        {
            return E_NOTIMPL;
        }

        HRESULT __stdcall GetTypeRefProps(mdTypeRef tr, mdToken * ptkResolutionScope, LPWSTR szName, ULONG cchName, ULONG * pchName) override
        {
            return E_NOTIMPL;
        }

        HRESULT __stdcall ResolveTypeRef(mdTypeRef tr, REFIID riid, IUnknown ** ppIScope, mdTypeDef * ptd) override
        {
            return E_NOTIMPL;
        }

        HRESULT __stdcall EnumMembers(HCORENUM * phEnum, mdTypeDef cl, mdToken rMembers[], ULONG cMax, ULONG * pcTokens) override
        {
            return E_NOTIMPL;
        }

        HRESULT __stdcall EnumMembersWithName(HCORENUM * phEnum, mdTypeDef cl, LPCWSTR szName, mdToken rMembers[], ULONG cMax, ULONG * pcTokens) override
        {
            return E_NOTIMPL;
        }

        HRESULT __stdcall EnumMethods(HCORENUM * phEnum, mdTypeDef cl, mdMethodDef rMethods[], ULONG cMax, ULONG * pcTokens) override
        {
            return E_NOTIMPL;
        }

        HRESULT __stdcall EnumMethodsWithName(HCORENUM * phEnum, mdTypeDef cl, LPCWSTR szName, mdMethodDef rMethods[], ULONG cMax, ULONG * pcTokens) override
        {
            return E_NOTIMPL;
        }

        HRESULT __stdcall EnumFields(HCORENUM * phEnum, mdTypeDef cl, mdFieldDef rFields[], ULONG cMax, ULONG * pcTokens) override
        {
            return E_NOTIMPL;
        }

        HRESULT __stdcall EnumFieldsWithName(HCORENUM * phEnum, mdTypeDef cl, LPCWSTR szName, mdFieldDef rFields[], ULONG cMax, ULONG * pcTokens) override
        {
            return E_NOTIMPL;
        }

        HRESULT __stdcall EnumParams(HCORENUM * phEnum, mdMethodDef mb, mdParamDef rParams[], ULONG cMax, ULONG * pcTokens) override
        {
            return E_NOTIMPL;
        }

        HRESULT __stdcall EnumMemberRefs(HCORENUM * phEnum, mdToken tkParent, mdMemberRef rMemberRefs[], ULONG cMax, ULONG * pcTokens) override
        {
            return E_NOTIMPL;
        }

        HRESULT __stdcall EnumMethodImpls(HCORENUM * phEnum, mdTypeDef td, mdToken rMethodBody[], mdToken rMethodDecl[], ULONG cMax, ULONG * pcTokens) override
        {
            return E_NOTIMPL;
        }

        HRESULT __stdcall EnumPermissionSets(HCORENUM * phEnum, mdToken tk, DWORD dwActions, mdPermission rPermission[], ULONG cMax, ULONG * pcTokens) override
        {
            return E_NOTIMPL;
        }

        HRESULT __stdcall FindMember(mdTypeDef td, LPCWSTR szName, PCCOR_SIGNATURE pvSigBlob, ULONG cbSigBlob, mdToken * pmb) override
        {
            return E_NOTIMPL;
        }

        HRESULT __stdcall FindMethod(mdTypeDef td, LPCWSTR szName, PCCOR_SIGNATURE pvSigBlob, ULONG cbSigBlob, mdMethodDef * pmb) override
        {
            return E_NOTIMPL;
        }

        HRESULT __stdcall FindField(mdTypeDef td, LPCWSTR szName, PCCOR_SIGNATURE pvSigBlob, ULONG cbSigBlob, mdFieldDef * pmb) override
        {
            return E_NOTIMPL;
        }

        HRESULT __stdcall FindMemberRef(mdTypeRef td, LPCWSTR szName, PCCOR_SIGNATURE pvSigBlob, ULONG cbSigBlob, mdMemberRef * pmr) override
        {
            return E_NOTIMPL;
        }

        HRESULT __stdcall GetMethodProps(mdMethodDef md, mdTypeDef* pClass, LPWSTR szMethod, ULONG cchMethod, ULONG* pchMethod, DWORD* pdwAttr, PCCOR_SIGNATURE* ppvSigBlob, ULONG* pcbSigBlob, ULONG* pulCodeRVA, DWORD* pdwImplFlags) override
        {
			return get_method_props(md, pClass, szMethod, cchMethod, pchMethod, pdwAttr, ppvSigBlob, pcbSigBlob, pulCodeRVA, pdwImplFlags);
        }

        HRESULT __stdcall GetMemberRefProps(mdMemberRef mr, mdToken * ptk, LPWSTR szMember, ULONG cchMember, ULONG * pchMember, PCCOR_SIGNATURE * ppvSigBlob, ULONG * pbSig) override
        {
            return E_NOTIMPL;
        }

        HRESULT __stdcall EnumProperties(HCORENUM * phEnum, mdTypeDef td, mdProperty rProperties[], ULONG cMax, ULONG * pcProperties) override
        {
            return E_NOTIMPL;
        }

        HRESULT __stdcall EnumEvents(HCORENUM * phEnum, mdTypeDef td, mdEvent rEvents[], ULONG cMax, ULONG * pcEvents) override
        {
            return E_NOTIMPL;
        }

        HRESULT __stdcall GetEventProps(mdEvent ev, mdTypeDef * pClass, LPCWSTR szEvent, ULONG cchEvent, ULONG * pchEvent, DWORD * pdwEventFlags, mdToken * ptkEventType, mdMethodDef * pmdAddOn, mdMethodDef * pmdRemoveOn, mdMethodDef * pmdFire, mdMethodDef rmdOtherMethod[], ULONG cMax, ULONG * pcOtherMethod) override
        {
            return E_NOTIMPL;
        }

        HRESULT __stdcall EnumMethodSemantics(HCORENUM * phEnum, mdMethodDef mb, mdToken rEventProp[], ULONG cMax, ULONG * pcEventProp) override
        {
            return E_NOTIMPL;
        }

        HRESULT __stdcall GetMethodSemantics(mdMethodDef mb, mdToken tkEventProp, DWORD * pdwSemanticsFlags) override
        {
            return E_NOTIMPL;
        }

        HRESULT __stdcall GetClassLayout(mdTypeDef td, DWORD * pdwPackSize, COR_FIELD_OFFSET rFieldOffset[], ULONG cMax, ULONG * pcFieldOffset, ULONG * pulClassSize) override
        {
            return E_NOTIMPL;
        }

        HRESULT __stdcall GetFieldMarshal(mdToken tk, PCCOR_SIGNATURE * ppvNativeType, ULONG * pcbNativeType) override
        {
            return E_NOTIMPL;
        }

        HRESULT __stdcall GetRVA(mdToken tk, ULONG * pulCodeRVA, DWORD * pdwImplFlags) override
        {
            return E_NOTIMPL;
        }

        HRESULT __stdcall GetPermissionSetProps(mdPermission pm, DWORD * pdwAction, void const ** ppvPermission, ULONG * pcbPermission) override
        {
            return E_NOTIMPL;
        }

        HRESULT __stdcall GetSigFromToken(mdSignature mdSig, PCCOR_SIGNATURE * ppvSig, ULONG * pcbSig) override
        {
            return E_NOTIMPL;
        }

        HRESULT __stdcall GetModuleRefProps(mdModuleRef mur, LPWSTR szName, ULONG cchName, ULONG * pchName) override
        {
            return E_NOTIMPL;
        }

        HRESULT __stdcall EnumModuleRefs(HCORENUM * phEnum, mdModuleRef rModuleRefs[], ULONG cmax, ULONG * pcModuleRefs) override
        {
            return E_NOTIMPL;
        }

        HRESULT __stdcall GetTypeSpecFromToken(mdTypeSpec typespec, PCCOR_SIGNATURE * ppvSig, ULONG * pcbSig) override
        {
            return E_NOTIMPL;
        }

        HRESULT __stdcall GetNameFromToken(mdToken tk, MDUTF8CSTR * pszUtf8NamePtr) override
        {
            return E_NOTIMPL;
        }

        HRESULT __stdcall EnumUnresolvedMethods(HCORENUM * phEnum, mdToken rMethods[], ULONG cMax, ULONG * pcTokens) override
        {
            return E_NOTIMPL;
        }

        HRESULT __stdcall GetUserString(mdString stk, LPWSTR szString, ULONG cchString, ULONG * pchString) override
        {
            return E_NOTIMPL;
        }

        HRESULT __stdcall GetPinvokeMap(mdToken tk, DWORD * pdwMappingFlags, LPWSTR szImportName, ULONG cchImportName, ULONG * pchImportName, mdModuleRef * pmrImportDLL) override
        {
            return E_NOTIMPL;
        }

        HRESULT __stdcall EnumSignatures(HCORENUM * phEnum, mdSignature rSignatures[], ULONG cmax, ULONG * pcSignatures) override
        {
            return E_NOTIMPL;
        }

        HRESULT __stdcall EnumTypeSpecs(HCORENUM * phEnum, mdTypeSpec rTypeSpecs[], ULONG cmax, ULONG * pcTypeSpecs) override
        {
            return E_NOTIMPL;
        }

        HRESULT __stdcall EnumUserStrings(HCORENUM * phEnum, mdString rStrings[], ULONG cmax, ULONG * pcStrings) override
        {
            return E_NOTIMPL;
        }

        HRESULT __stdcall GetParamForMethodIndex(mdMethodDef md, ULONG ulParamSeq, mdParamDef * ppd) override
        {
            return E_NOTIMPL;
        }

        HRESULT __stdcall EnumCustomAttributes(HCORENUM * phEnum, mdToken tk, mdToken tkType, mdCustomAttribute rCustomAttributes[], ULONG cMax, ULONG * pcCustomAttributes) override
        {
            return E_NOTIMPL;
        }

        HRESULT __stdcall GetCustomAttributeProps(mdCustomAttribute cv, mdToken * ptkObj, mdToken * ptkType, void const ** ppBlob, ULONG * pcbSize) override
        {
            return E_NOTIMPL;
        }

        HRESULT __stdcall FindTypeRef(mdToken tkResolutionScope, LPCWSTR szName, mdTypeRef * ptr) override
        {
            return E_NOTIMPL;
        }

        HRESULT __stdcall GetMemberProps(mdToken mb, mdTypeDef * pClass, LPWSTR szMember, ULONG cchMember, ULONG * pchMember, DWORD * pdwAttr, PCCOR_SIGNATURE * ppvSigBlob, ULONG * pcbSigBlob, ULONG * pulCodeRVA, DWORD * pdwImplFlags, DWORD * pdwCPlusTypeFlag, UVCP_CONSTANT * ppValue, ULONG * pcchValue) override
        {
            return E_NOTIMPL;
        }

        HRESULT __stdcall GetFieldProps(mdFieldDef mb, mdTypeDef * pClass, LPWSTR szField, ULONG cchField, ULONG * pchField, DWORD * pdwAttr, PCCOR_SIGNATURE * ppvSigBlob, ULONG * pcbSigBlob, DWORD * pdwCPlusTypeFlag, UVCP_CONSTANT * ppValue, ULONG * pcchValue) override
        {
            return E_NOTIMPL;
        }

        HRESULT __stdcall GetPropertyProps(mdProperty prop, mdTypeDef * pClass, LPCWSTR szProperty, ULONG cchProperty, ULONG * pchProperty, DWORD * pdwPropFlags, PCCOR_SIGNATURE * ppvSig, ULONG * pbSig, DWORD * pdwCPlusTypeFlag, UVCP_CONSTANT * ppDefaultValue, ULONG * pcchDefaultValue, mdMethodDef * pmdSetter, mdMethodDef * pmdGetter, mdMethodDef rmdOtherMethod[], ULONG cMax, ULONG * pcOtherMethod) override
        {
            return E_NOTIMPL;
        }

        HRESULT __stdcall GetParamProps(mdParamDef tk, mdMethodDef * pmd, ULONG * pulSequence, LPWSTR szName, ULONG cchName, ULONG * pchName, DWORD * pdwAttr, DWORD * pdwCPlusTypeFlag, UVCP_CONSTANT * ppValue, ULONG * pcchValue) override
        {
            return E_NOTIMPL;
        }

        HRESULT __stdcall GetCustomAttributeByName(mdToken tkObj, LPCWSTR szName, const void ** ppData, ULONG * pcbData) override
        {
            return E_NOTIMPL;
        }

        BOOL __stdcall IsValidToken(mdToken tk) override
        {
            return 0;
        }

        HRESULT __stdcall GetNestedClassProps(mdTypeDef tdNestedClass, mdTypeDef * ptdEnclosingClass) override
        {
            return E_NOTIMPL;
        }

        HRESULT __stdcall GetNativeCallConvFromSig(void const * pvSig, ULONG cbSig, ULONG * pCallConv) override
        {
            return E_NOTIMPL;
        }

        HRESULT __stdcall IsGlobal(mdToken pd, int * pbGlobal) override
        {
            return E_NOTIMPL;
        }

		#pragma endregion

		#pragma region IMetaDataImport2

        HRESULT __stdcall EnumGenericParams(HCORENUM * phEnum, mdToken tk, mdGenericParam rGenericParams[], ULONG cMax, ULONG * pcGenericParams) override
        {
            return E_NOTIMPL;
        }

        HRESULT __stdcall GetGenericParamProps(mdGenericParam gp, ULONG * pulParamSeq, DWORD * pdwParamFlags, mdToken * ptOwner, DWORD * reserved, LPWSTR wzname, ULONG cchName, ULONG * pchName) override
        {
            return E_NOTIMPL;
        }
        
        HRESULT __stdcall GetMethodSpecProps(mdMethodSpec mi, mdToken * tkParent, PCCOR_SIGNATURE * ppvSigBlob, ULONG * pcbSigBlob) override
        {
            return E_NOTIMPL;
        }
        
        HRESULT __stdcall EnumGenericParamConstraints(HCORENUM * phEnum, mdGenericParam tk, mdGenericParamConstraint rGenericParamConstraints[], ULONG cMax, ULONG * pcGenericParamConstraints) override
        {
            return E_NOTIMPL;
        }
        
        HRESULT __stdcall GetGenericParamConstraintProps(mdGenericParamConstraint gpc, mdGenericParam * ptGenericParam, mdToken * ptkConstraintType) override
        {
            return E_NOTIMPL;
        }
        
        HRESULT __stdcall GetPEKind(DWORD * pdwPEKind, DWORD * pdwMAchine) override
        {
            return E_NOTIMPL;
        }
        
        HRESULT __stdcall GetVersionString(LPWSTR pwzBuf, DWORD ccBufSize, DWORD * pccBufSize) override
        {
            return E_NOTIMPL;
        }
        
        HRESULT __stdcall EnumMethodSpecs(HCORENUM * phEnum, mdToken tk, mdMethodSpec rMethodSpecs[], ULONG cMax, ULONG * pcMethodSpecs) override
        {
            return E_NOTIMPL;
        }

		#pragma endregion

		#pragma region IMetaDataAssemblyImport

		function<HRESULT(mdAssembly*)> get_assembly_from_scope = [](mdAssembly*) { return S_OK; };
		function<HRESULT(mdAssembly, const void**, ULONG*, ULONG*, LPWSTR, ULONG, ULONG*, ASSEMBLYMETADATA*, DWORD*)> get_assembly_props =
			[](mdAssembly, const void**, ULONG*, ULONG*, LPWSTR, ULONG, ULONG*, ASSEMBLYMETADATA*, DWORD*) { return E_NOTIMPL; };

		HRESULT __stdcall GetAssemblyProps(mdAssembly assembly, const void** public_key, ULONG* public_key_size, ULONG* hash_algorithm, LPWSTR name, ULONG name_capacity, ULONG* name_size, ASSEMBLYMETADATA* metadata, DWORD* assembly_flags) override
		{
			return this->get_assembly_props(assembly, public_key, public_key_size, hash_algorithm, name, name_capacity, name_size, metadata, assembly_flags);
		}

		HRESULT __stdcall GetAssemblyRefProps(mdAssemblyRef mdar, const void ** ppbPublicKeyOrToken, ULONG * pcbPublicKeyOrToken, LPWSTR szName, ULONG cchName, ULONG * pchName, ASSEMBLYMETADATA * pMetaData, const void ** ppbHashValue, ULONG * pcbHashValue, DWORD * pdwAssemblyRefFlags) override
		{
			return E_NOTIMPL;
		}

		HRESULT __stdcall GetFileProps(mdFile mdf, LPWSTR szName, ULONG cchName, ULONG * pchName, const void ** ppbHashValue, ULONG * pcbHashValue, DWORD * pdwFileFlags) override
		{
			return E_NOTIMPL;
		}

		HRESULT __stdcall GetExportedTypeProps(mdExportedType mdct, LPWSTR szName, ULONG cchName, ULONG * pchName, mdToken * ptkImplementation, mdTypeDef * ptkTypeDef, DWORD * pdwExportedTypeFlags) override
		{
			return E_NOTIMPL;
		}

		HRESULT __stdcall GetManifestResourceProps(mdManifestResource mdmr, LPWSTR szName, ULONG cchName, ULONG * pchName, mdToken * ptkImplementation, DWORD * pdwOffset, DWORD * pdwResourceFlags) override
		{
			return E_NOTIMPL;
		}

		HRESULT __stdcall EnumAssemblyRefs(HCORENUM * phEnum, mdAssemblyRef rAssemblyRefs[], ULONG cMax, ULONG * pcTokens) override
		{
			return E_NOTIMPL;
		}

		HRESULT __stdcall EnumFiles(HCORENUM * phEnum, mdFile rFiles[], ULONG cMax, ULONG * pcTokens) override
		{
			return E_NOTIMPL;
		}

		HRESULT __stdcall EnumExportedTypes(HCORENUM * phEnum, mdExportedType rExportedTypes[], ULONG cMax, ULONG * pcTokens) override
		{
			return E_NOTIMPL;
		}

		HRESULT __stdcall EnumManifestResources(HCORENUM * phEnum, mdManifestResource rManifestResources[], ULONG cMax, ULONG * pcTokens) override
		{
			return E_NOTIMPL;
		}

		HRESULT __stdcall GetAssemblyFromScope(mdAssembly* assembly) override
		{
			return this->get_assembly_from_scope(assembly);
		}

		HRESULT __stdcall FindExportedTypeByName(LPCWSTR szName, mdToken mdtExportedType, mdExportedType * ptkExportedType) override
		{
			return E_NOTIMPL;
		}

		HRESULT __stdcall FindManifestResourceByName(LPCWSTR szName, mdManifestResource * ptkManifestResource) override
		{
			return E_NOTIMPL;
		}

		void __stdcall IMetaDataAssemblyImport::CloseEnum(HCORENUM hEnum) override
		{
			return this->close_enum(hEnum);
		}

		HRESULT __stdcall FindAssembliesByName(LPCWSTR szAppBase, LPCWSTR szPrivateBin, LPCWSTR szAssemblyName, IUnknown * ppIUnk[], ULONG cMax, ULONG * pcAssemblies) override
		{
			return E_NOTIMPL;
		}

		#pragma endregion
	};
}}}
