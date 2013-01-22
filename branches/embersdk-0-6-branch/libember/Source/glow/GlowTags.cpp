/*
 * Explicitly undefine the macro and include the implementation file manually afterwards.
 * This is required in order to avoid multiply defined symbols when linking because of the
 * definition being transitively set in headers indirectly included.
 */
#ifdef LIBEMBER_HEADER_ONLY
#  undef LIBEMBER_HEADER_ONLY
#endif
#include "ember/glow/GlowTags.hpp"
#include "ember/glow/impl/GlowTags.ipp"
