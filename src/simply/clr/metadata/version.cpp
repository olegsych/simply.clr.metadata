#include "version.h"

namespace simply { namespace clr { namespace metadata
{
    bool operator==(const version& left, const version& right)
    {
        return left.major() == right.major()
            && left.minor() == right.minor()
            && left.build() == right.build()
            && left.revision() == right.revision();
    }

    bool operator<(const version& left, const version& right)
    {
        if (left.major() < right.major())
        {
            return true;
        }
        else if (left.major() > right.major())
        {
            return false;
        }

        if (left.minor() < right.minor())
        {
            return true;
        }
        else if (left.minor() > right.minor())
        {
            return false;
        }

        if (left.build() < right.build())
        {
            return true;
        }
        else if (left.build() > right.build())
        {
            return false;
        }

        return left.revision() < right.revision();
    }
}}}