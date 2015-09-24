#include <simply/com.h>
#include "metadata_enumerator.h"
#include "type_definitions.h"

using namespace simply;
using namespace std;

namespace simply { namespace clr { namespace metadata { namespace implementation
{
	class type_definitions::enumerator : public metadata_enumerator<type>
	{
	public:
		enumerator(simply::com_ptr<IMetaDataImport2> metadata)
			: metadata_enumerator<type>(metadata)
		{
		}

		bool get_next(type* element) override
		{
			mdTypeDef token;
			ULONG count;
			check(metadata->EnumTypeDefs(&enum_handle, &token, 1, &count));
			if (count == 1)
			{
				new(element) type { token, metadata };
				return true;
			}

			return false;
		}
	};

	type_definitions::type_definitions(com_ptr<IMetaDataImport2> metadata)
		: _metadata { metadata }
	{
		if (!metadata.get())
		{
			throw invalid_argument { "metadata must not be a nullptr" };
		}
	}

	unique_ptr<enumerator<type>> type_definitions::create_enumerator()
	{
		return unique_ptr<simply::enumerator<type>> { new type_definitions::enumerator { _metadata } };
	}
}}}}
