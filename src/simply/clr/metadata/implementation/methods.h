#include <memory>
#include <simply/collections/enumerable.h>
#include <simply/com/com_ptr.h>
#include "../method.h"

namespace simply { namespace clr { namespace metadata { namespace implementation
{
	class methods : public enumerable<method>
	{
	public:
		methods(mdTypeDef type_token, com_ptr<IMetaDataImport2>);
		std::unique_ptr<enumerator<method>> create_enumerator() override;
	private:
		const mdTypeDef type_token;
		const com_ptr<IMetaDataImport2> metadata;
	};
}}}}