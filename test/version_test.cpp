#include <CppUnitTest.h>
#include <simply/assert.h>
#include <simply/clr/metadata/version.h>

using namespace std;

namespace simply { namespace clr { namespace metadata
{
    TEST_CLASS(version_test)
    {
    public:
        TEST_METHOD(major_returns_value_specified_in_constructor)
        {
            unsigned short major { 42 };

            version sut { major, 0, 0, 0 };

            assert::is_equal(major, sut.major());
        }

        TEST_METHOD(minor_returns_value_specified_in_constructor)
        {
            unsigned short minor { 42 };

            version sut { 0, minor, 0, 0 };

            assert::is_equal(minor, sut.minor());
        }

        TEST_METHOD(build_returns_value_specified_in_constructor)
        {
            unsigned short build { 42 };

            version sut { 0, 0, build, 0 };

            assert::is_equal(build, sut.build());
        }

        TEST_METHOD(revision_returns_value_specified_in_constructor)
        {
            unsigned short revision { 42 };

            version sut { 0, 0, 0, revision };

            assert::is_equal(revision, sut.revision());
        }

        TEST_METHOD(copy_constructor_copies_values_from_given_version)
        {
            version source { 1, 2, 3, 4 };

            version sut { source };

            assert::is_equal(source.major(), sut.major());
            assert::is_equal(source.minor(), sut.minor());
            assert::is_equal(source.build(), sut.build());
            assert::is_equal(source.revision(), sut.revision());
        }

        TEST_METHOD(assignment_copies_values_from_given_version)
        {
            version source { 1, 2, 3, 4 };

            version sut = source;

            assert::is_equal(source.major(), sut.major());
            assert::is_equal(source.minor(), sut.minor());
            assert::is_equal(source.build(), sut.build());
            assert::is_equal(source.revision(), sut.revision());
        }

        TEST_METHOD(equals_operator_returns_boolean_indicating_whether_two_versions_are_same)
        {
            assert::is_true(version { 0, 0, 0, 0 } == version { 0, 0, 0, 0 });
            assert::is_false(version { 1, 0, 0, 0 } == version { 2, 0, 0, 0 });
            assert::is_false(version { 0, 1, 0, 0 } == version { 0, 2, 0, 0 });
            assert::is_false(version { 0, 0, 1, 0 } == version { 0, 0, 2, 0 });
            assert::is_false(version { 0, 0, 0, 1 } == version { 0, 0, 0, 2 });
        }

        TEST_METHOD(not_equal_operator_returns_boolean_indicating_whether_two_versions_are_different)
        {
            assert::is_true(version { 1, 0, 0, 0 } != version { 2, 0, 0, 0 });
            assert::is_true(version { 0, 1, 0, 0 } != version { 0, 2, 0, 0 });
            assert::is_true(version { 0, 0, 1, 0 } != version { 0, 0, 2, 0 });
            assert::is_true(version { 0, 0, 0, 1 } != version { 0, 0, 0, 2 });
            assert::is_false(version { 0, 0, 0, 0 } != version { 0, 0, 0, 0 });
        }

        TEST_METHOD(less_than_operator_returns_boolean_indicating_whether_left_version_is_smaller_than_right)
        {
            assert::is_true(version { 1, 9, 9, 9 } < version { 2, 0, 0, 0 });
            assert::is_true(version { 0, 1, 9, 9 } < version { 0, 2, 0, 0 });
            assert::is_true(version { 0, 0, 1, 9 } < version { 0, 0, 2, 0 });
            assert::is_true(version { 0, 0, 0, 1 } < version { 0, 0, 0, 2 });
            assert::is_false(version { 2, 0, 0, 0 } < version { 2, 0, 0, 0 });
            assert::is_false(version { 2, 0, 0, 0 } < version { 1, 9, 9, 9 });
            assert::is_false(version { 0, 2, 0, 0 } < version { 0, 1, 9, 9 });
            assert::is_false(version { 0, 0, 2, 0 } < version { 0, 0, 1, 9 });
            assert::is_false(version { 0, 0, 0, 2 } < version { 0, 0, 0, 1 });
        }

        TEST_METHOD(greater_than_operator_returns_boolean_indicating_whether_left_version_is_greater_than_right)
        {
            assert::is_true(version { 2, 0, 0, 0 } > version { 1, 9, 9, 9 });
            assert::is_true(version { 0, 2, 0, 0 } > version { 0, 1, 9, 9 });
            assert::is_true(version { 0, 0, 2, 0 } > version { 0, 0, 1, 9 });
            assert::is_true(version { 0, 0, 0, 2 } > version { 0, 0, 0, 1 });
            assert::is_false(version { 2, 0, 0, 0 } > version { 2, 0, 0, 0 });
            assert::is_false(version { 1, 9, 9, 9 } > version { 2, 0, 0, 0 });
            assert::is_false(version { 0, 1, 9, 9 } > version { 0, 2, 0, 0 });
            assert::is_false(version { 0, 0, 1, 9 } > version { 0, 0, 2, 0 });
        }

        TEST_METHOD(less_than_or_equal_operator_returns_boolean_indicating_whether_left_version_is_less_or_equal_to_right)
        {
            assert::is_true(version { 1, 9, 9, 9 } <= version { 2, 0, 0, 0 });
            assert::is_true(version { 2, 0, 0, 0 } <= version { 2, 0, 0, 0 });
            assert::is_false(version { 2, 0, 0, 0 } <= version { 1, 9, 9, 9 });
        }

        TEST_METHOD(greater_than_or_equal_operator_returns_boolean_indicating_whether_left_version_is_greater_or_equal_to_right)
        {
            assert::is_true(version { 2, 0, 0, 0 } >= version { 1, 9, 9, 9 });
            assert::is_true(version { 2, 0, 0, 0 } >= version { 2, 0, 0, 0 });
            assert::is_false(version { 1, 9, 9, 9 } >= version { 2, 0, 0, 0 });
        }

        TEST_METHOD(stream_insertion_operator_writes_version_to_given_stream)
        {
            ostringstream stream;

            stream << version { 1, 2, 3, 4 };

            assert::is_equal("1.2.3.4", stream.str());
        }
    };
}}}
