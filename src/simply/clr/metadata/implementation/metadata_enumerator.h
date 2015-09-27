#pragma once

#pragma warning(disable: 4091)
#include <cor.h>
#pragma warning(default: 4091)
#include <stdexcept>
#include <simply/com/com_ptr.h>
#include <simply/collections/enumerator.h>

namespace simply { namespace clr { namespace metadata { namespace implementation 
{
	template<typename element_t> class metadata_enumerator : public enumerator<element_t>
	{
	protected:
		const com_ptr<IMetaDataImport2> metadata;
		HCORENUM enum_handle;
	public:
		metadata_enumerator(com_ptr<IMetaDataImport2> metadata)
			: metadata { metadata }, enum_handle { nullptr }
		{
			if (!metadata.get())
			{
				throw std::invalid_argument { "metadata must not be a nullptr" };
			}
		}

		~metadata_enumerator() override
		{
			if (enum_handle)
			{
				metadata->CloseEnum(enum_handle);
			}
		}
	};
}}}}