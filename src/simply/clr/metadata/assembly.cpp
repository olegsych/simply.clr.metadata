#include "assembly.h"
#include "implementation.h"
#include "implementation/type_definitions.h"

namespace simply { namespace clr { namespace metadata
{
	using namespace std;
	using namespace implementation;

    namespace // implementation
    {
        hash_algorithm decode_hash_algorithm(unsigned long raw_value)
        {
            switch (raw_value)
            {
                case 32771: return hash_algorithm::md5;
                case 32772: return hash_algorithm::sha1;
                case 32780: return hash_algorithm::sha256;
                case 32781: return hash_algorithm::sha384;
                case 32782: return hash_algorithm::sha512;
                default: return hash_algorithm::none;
            }
        }

        processor_architecture decode_processor_architecture(unsigned long raw_value)
        {
            return static_cast<processor_architecture>((raw_value & CorAssemblyFlags::afPA_Mask) >> CorAssemblyFlags::afPA_Shift);
        }
    }

    assembly::assembly(com_ptr<IMetaDataAssemblyImport> metadata)
        : _metadata { metadata }
    {
    }

    assembly_identity assembly::identity() const
    {
        const void* public_key;
        unsigned long public_key_size;
        unsigned long hash_algorithm;
        const unsigned long name_capacity = 1024;
        wchar_t name_buffer[name_capacity];
        unsigned long name_length;
        ASSEMBLYMETADATA metadata;
        unsigned long flags;

		check(_metadata->GetAssemblyProps(token(), &public_key, &public_key_size, &hash_algorithm, name_buffer, name_capacity, &name_length, &metadata, &flags));
        return assembly_identity
        {
            wstring { name_buffer, name_length },
            decode_hash_algorithm(hash_algorithm),
            decode_processor_architecture(flags),
            version { metadata.usMajorVersion, metadata.usMinorVersion, metadata.usBuildNumber, metadata.usRevisionNumber },
            vector<unsigned char> { reinterpret_cast<const unsigned char*>(public_key), reinterpret_cast<const unsigned char*>(public_key) + public_key_size }
        };
    }

	unsigned int assembly::token() const
	{
		mdAssembly token;
		check(_metadata->GetAssemblyFromScope(&token));
		return token;
	}

	range<type> assembly::types() const
	{
		com_ptr<IMetaDataImport2> metadata;
		check(_metadata->QueryInterface(IID_IMetaDataImport2, metadata));
		return range<type> { shared_ptr<enumerable<type>> { new type_definitions { metadata } } };
	}

	bool assembly::operator==(const assembly& other) const
	{
		return _metadata.get() == other._metadata.get();
	}

    assembly assembly::load_from(const wstring& file_path)
    {
        com_ptr<IMetaDataDispenserEx> dispenser { implementation::create_dispenser() };
        com_ptr<IMetaDataAssemblyImport> metadata;
        check(dispenser->OpenScope(file_path.c_str(), CorOpenFlags::ofReadOnly, IID_IMetaDataAssemblyImport, metadata));
        return assembly { metadata };
    }
}}}

