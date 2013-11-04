#ifndef __LIBEMBER_IMPL_VERSION_IPP
#define __LIBEMBER_IMPL_VERSION_IPP

#define _LIBEMBER_MAKE_VERSION(MAJ, MIN, REV)       _LIBEMBER_MAKE_VERSION_IMPL(MAJ, MIN, REV)
#define _LIBEMBER_MAKE_VERSION_IMPL(MAJ, MIN, REV)  ("v" #MAJ "." #MIN "." #REV)            

namespace libember
{
    LIBEMBER_EXTERN
    char const* const versionString = _LIBEMBER_MAKE_VERSION(LIBEMBER_VERSION_MAJOR, LIBEMBER_VERSION_MINOR, LIBEMBER_VERSION_REVISION);
}

#undef _LIBEMBER_MAKE_VERSION
#undef _LIBEMBER_MAKE_VERSION_IMPL

#endif  // __LIBEMBER_IMPL_VERSION_IPP
