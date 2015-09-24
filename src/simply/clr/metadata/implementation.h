#pragma once

#include <functional>
#include <cor.h>
#include <simply/com.h>

namespace simply { namespace clr { namespace metadata { namespace implementation 
{
    extern std::function<com_ptr<IMetaDataDispenserEx>(void)> create_dispenser;
}}}}