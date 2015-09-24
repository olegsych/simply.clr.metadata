#pragma once

#include <ostream>
#include <utility>

namespace simply { namespace clr { namespace metadata 
{
    using namespace std::rel_ops;

    class version
    {
    public:
        inline version(unsigned short major, unsigned short minor, unsigned short build, unsigned short revision)
            : _major { major }, _minor { minor }, _build { build }, _revision { revision }
        {
        }

        inline unsigned short major() const
        {
            return _major;
        }

        inline unsigned short minor() const
        {
            return _minor;
        }

        inline unsigned short build() const
        {
            return _build;
        }

        inline unsigned short revision() const
        {
            return _revision;
        }

    private:
        const unsigned short _major;
        const unsigned short _minor;
        const unsigned short _build;
        const unsigned short _revision;
    };

    bool operator==(const version& left, const version& right);
    bool operator<(const version& left, const version& right);

    template <class char_t, class traits>
    std::basic_ostream<char_t, traits>& operator<<(std::basic_ostream<char_t, traits>& stream, const version& version)
    {
        return stream << version.major() << '.' << version.minor() << '.' << version.build() << '.' << version.revision();
    }
}}}