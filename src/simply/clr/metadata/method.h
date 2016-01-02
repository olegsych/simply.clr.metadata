#pragma once

#include <string>
#include <simply/com/com_ptr.h>
#include "implementation/interop.h"

namespace simply { namespace clr { namespace metadata
{
	class method
	{
	public:
		method(mdMethodDef, com_ptr<IMetaDataImport2>);
		const std::wstring name() const;
		unsigned int token() const;
		bool operator==(const method&) const;

	private:
		const mdMethodDef _token;
		const com_ptr<IMetaDataImport2> _metadata;
	};
}}}
