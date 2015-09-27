#pragma once

#include <memory>
#include <simply/collections/enumerable.h>
#include <simply/com/com_ptr.h>
#include "../type.h"

namespace simply { namespace clr { namespace metadata { namespace implementation
{
	class types: public enumerable<type>
	{
	public:
		types(com_ptr<IMetaDataImport2> metadata);
		std::unique_ptr<enumerator<type>> create_enumerator() override;
	private:
		const com_ptr<IMetaDataImport2> metadata;
	};
}}}}