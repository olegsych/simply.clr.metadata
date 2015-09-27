#pragma once

#include <memory>
#include <simply/com/com_ptr.h>
#include "metadata_enumerable.h"
#include "../type.h"

namespace simply { namespace clr { namespace metadata { namespace implementation
{
	class types: public metadata_enumerable<type>
	{
	public:
		types(com_ptr<IMetaDataImport2> metadata);
		std::unique_ptr<enumerator<type>> create_enumerator() override;
	};
}}}}