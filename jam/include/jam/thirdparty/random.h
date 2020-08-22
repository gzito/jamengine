#ifndef __JAM_RANDOM_H__

#include <jam/jam-config.h>

// Windows header file - specifically the WinDef.h - defines two macros min and max
// which may result in conflicts and compiler errors
#ifdef min
#undef min
#endif

#ifdef max
#undef max
#endif

#include <effolkronium/random.hpp>

namespace jam {
#ifndef JAM_MULTITHREADING_ENABLED
    using Random = effolkronium::random_static ;
#else
    using Random = effolkronium::random_thread_local6 ;
#endif
}

#endif
