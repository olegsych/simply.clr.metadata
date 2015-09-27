#pragma once

#pragma warning(disable: 4091)
#include <cor.h>
#pragma warning(default: 4091)
#include <stdexcept>
#include <simply/com/com_ptr.h>
#include <simply/collections/enumerable.h>

namespace simply { namespace clr { namespace metadata { namespace implementation 
{
	template<typename element_t> class metadata_enumerable: public enumerable<element_t>
	{
	protected:
		const com_ptr<IMetaDataImport2> metadata;
	public:
		metadata_enumerable(com_ptr<IMetaDataImport2> metadata)
			: metadata { metadata }
		{
			if (!metadata.get())
			{
				throw std::invalid_argument { "metadata must not be a nullptr." };
			}
		}
	};
}}}}