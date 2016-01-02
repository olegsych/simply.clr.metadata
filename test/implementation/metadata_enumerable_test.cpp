#include <CppUnitTest.h>
#include <simply/assert.h>
#include <simply/com.h>
#include <simply/clr/metadata/implementation/metadata_enumerable.h>
#include "../stub_metadata.h"

using namespace std;

namespace simply { namespace clr { namespace metadata { namespace implementation
{
	TEST_CLASS(metadata_enumerable_test)
	{
		class testable_metadata_enumerable : public metadata_enumerable<int>
		{
		public:
			testable_metadata_enumerable(com_ptr<IMetaDataImport2> metadata)
				: metadata_enumerable<int>(metadata)
			{
			}

			std::unique_ptr<enumerator<int>> create_enumerator() override
			{
				throw logic_error { "not implemented" };
			}

			com_ptr<IMetaDataImport2> metadata()
			{
				return metadata_enumerable<int>::metadata;
			}
		};
	public:
		TEST_METHOD(constructor_throws_invalid_argument_when_IMetaDataImport2_is_nullptr)
		{
			com_ptr<IMetaDataImport2> metadata;
			auto e = assert::throws<invalid_argument>([&] { testable_metadata_enumerable { metadata }; });
			assert::is_equal("metadata must not be a nullptr.", e->what());
		}

		TEST_METHOD(constructor_stores_IMetaDataImport2_for_use_by_derived_classes)
		{
			stub_metadata metadata;
			testable_metadata_enumerable sut { com_ptr<IMetaDataImport2> { &metadata } };
			assert::is_equal<IMetaDataImport2*>(&metadata, sut.metadata().get());
		}
	};
}}}}
