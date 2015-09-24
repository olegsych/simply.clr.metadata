#include "stdafx.h"
#include <CppUnitTest.h>
#include <simply/assert.h>
#include <simply/com.h>
#include <simply/clr/metadata/implementation/metadata_enumerator.h>
#include "../stub_metadata.h"

using namespace std;

namespace simply { namespace clr { namespace metadata { namespace implementation
{
	TEST_CLASS(metadata_enumerator_test)
	{
		class testable_metadata_enumerator : public metadata_enumerator<int>
		{
		public:
			testable_metadata_enumerator(com_ptr<IMetaDataImport2> metadata)
				: metadata_enumerator<int>(metadata)
			{
			}

			HCORENUM enum_handle() const
			{
				return metadata_enumerator<int>::enum_handle;
			}

			void enum_handle(HCORENUM value)
			{
				metadata_enumerator<int>::enum_handle = value;
			}

			bool get_next(int* element) override
			{
				return false;
			}
		};

	public:
		TEST_METHOD(class_inherits_from_enumerator_to_support_metadata_range_types)
		{
			assert::is_base_of<enumerator<int>, metadata_enumerator<int>>();
		}

		TEST_METHOD(constructor_throws_invalid_argument_when_metadata_object_is_not_specified)
		{
			com_ptr<IMetaDataImport2> metadata;
			auto e = assert::throws<invalid_argument>([&] { testable_metadata_enumerator { metadata }; });
			assert::find("metadata must not be a nullptr", e->what());
		}

		TEST_METHOD(constructor_initializes_enum_handle_for_use_by_concrete_subclasses)
		{
			stub_metadata metadata;
			testable_metadata_enumerator sut { com_ptr<IMetaDataImport2> { &metadata} };
			assert::is_null(sut.enum_handle());
		}

		TEST_METHOD(destructor_closes_previously_created_enum_handle_to_free_resources)
		{
			HCORENUM expected_value = reinterpret_cast<HCORENUM>(42);
			HCORENUM actual_value { nullptr };
			stub_metadata metadata;
			metadata.close_enum = [&](HCORENUM e) { actual_value = e; };
			testable_metadata_enumerator sut { com_ptr<IMetaDataImport2> { &metadata} };
			sut.enum_handle(expected_value);		

			sut.~testable_metadata_enumerator();
			
			assert::is_equal(expected_value, actual_value);
		}

		TEST_METHOD(destructor_doesnt_attempt_to_close_enum_handle_that_wasnt_created)
		{
			bool enum_closed { false };
			stub_metadata metadata;
			metadata.close_enum = [&](HCORENUM e) { enum_closed = true; };
			testable_metadata_enumerator sut{ com_ptr<IMetaDataImport2> { &metadata} };

			sut.~testable_metadata_enumerator();

			assert::is_false(enum_closed);
		}
	};
}}}}
