#pragma once

#pragma warning(disable: 4091)
#include <cor.h>
#pragma warning(default: 4091)
#include <string>
#include <simply/com/com_ptr.h>

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
