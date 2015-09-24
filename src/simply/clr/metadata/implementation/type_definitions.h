#pragma once

#include <memory>
#include <simply/enumerable.h>
#include <simply/com/com_ptr.h>
#include "../type.h"

namespace simply { namespace clr { namespace metadata { namespace implementation
{
	class type_definitions : public enumerable<type>
	{
	public:
		type_definitions(com_ptr<IMetaDataImport2> metadata);
		std::unique_ptr<enumerator<type>> create_enumerator() override;
	private:
		const com_ptr<IMetaDataImport2> _metadata;
		class enumerator;
	};
}}}}