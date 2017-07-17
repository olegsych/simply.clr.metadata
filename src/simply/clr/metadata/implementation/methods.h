#include <memory>
#include <simply/com/com_ptr.h>
#include "metadata_enumerable.h"
#include "../method.h"

namespace simply { namespace clr { namespace metadata { namespace implementation
{
    class methods : public metadata_enumerable<method>
    {
    public:
        methods(mdTypeDef type_token, com_ptr<IMetaDataImport2>);
        std::unique_ptr<enumerator<method>> create_enumerator() override;
    private:
        const mdTypeDef type_token;
    };
}}}}