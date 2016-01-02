#pragma once

#include <functional>
#include <simply/com.h>
#include "implementation/interop.h"

namespace simply { namespace clr { namespace metadata { namespace implementation
{
    extern std::function<com_ptr<IMetaDataDispenserEx>(void)> create_dispenser;
}}}}
