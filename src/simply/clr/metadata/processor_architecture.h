#pragma once

#include <ostream>

namespace simply { namespace clr { namespace metadata
{
    /// <summary>
    /// Identifies the processor and bits-per-word of the platform targeted by an executable.
    /// </summary>
    enum class processor_architecture
    {
        /// <summary>
        /// An unknown or unspecified combination of processor and bits-per-word.
        /// </summary>
        none,

        /// <summary>
        /// Neutral with respect to processor and bits-per-word.
        /// </summary>
        msil,

        /// <summary>
        /// A 32-bit Intel processor, either native or in the Windows on Windows environment on a 
        /// 64-bit platform (WOW64).
        /// </summary>
        x86,

        /// <summary>
        /// A 64-bit Intel processor only.
        /// </summary>
        ia64,

        /// <summary>
        /// A 64-bit AMD processor only.
        /// </summary>
        amd64,

        /// <summary>
        /// An ARM processor.
        /// </summary>
        arm
    };

    template <class char_t, class traits>
    std::basic_ostream<char_t, traits>& operator<<(std::basic_ostream<char_t, traits>& stream, const processor_architecture value)
    {
        switch (value)
        {
            case processor_architecture::none:
                stream << "none";
                break;
            case processor_architecture::msil:
                stream << "msil";
                break;
            case processor_architecture::x86:
                stream << "x86";
                break;
            case processor_architecture::ia64:
                stream << "ia64";
                break;
            case processor_architecture::amd64:
                stream << "amd64";
                break;
            case processor_architecture::arm:
                stream << "arm";
                break;
            default:
                stream << static_cast<int>(value);
                break;
        }

        return stream;
    }
}}}
