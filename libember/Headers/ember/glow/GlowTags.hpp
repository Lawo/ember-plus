#ifndef __LIBEMBER_GLOW_GLOWTAGS_HPP
#define __LIBEMBER_GLOW_GLOWTAGS_HPP

#include "../util/Api.hpp"
#include "GlowProperty.hpp"

namespace libember { namespace glow
{ 
    /**
     * Lists all application specific tags that are used by glow to store properties of 
     * nodes, parameters and other object types.
     */
    struct LIBEMBER_API GlowTags
    {
        /**
         * Returns the application tag identifying a number.
         * @return Returns the application tag identifying a number.
         */
        static ber::Tag Number(); 

        /**
         * Returns the application tag identifying an identifier.
         * @return Returns the application tag identifying an identifier.
         */
        static ber::Tag Identifier();

        /**
         * Returns the application tag identifying a number.
         * @return Returns the application tag identifying a number.
         */
        static ber::Tag Description();

        /**
         * Returns the application tag identifying a value.
         * @return Returns the application tag identifying a value.
         */
        static ber::Tag Value();

        /**
         * Returns the application tag identifying a minimum.
         * @return Returns the application tag identifying a minimum.
         */
        static ber::Tag Minimum();

        /**
         * Returns the application tag identifying a maximum.
         * @return Returns the application tag identifying a maximum.
         */
        static ber::Tag Maximum();

        /**
         * Returns the application tag identifying the writeable flag.
         * @return Returns the application tag identifying the writeable flag.
         */
        static ber::Tag IsWriteable();

        /**
         * Returns the application tag identifying a format string.
         * @return Returns the application tag identifying a format string.
         */
        static ber::Tag Format();

        /**
         * Returns the application tag identifying an enumeration.
         * @return Returns the application tag identifying an enumeration.
         */
        static ber::Tag Enumeration();

        /**
         * Returns the application tag identifying a factor.
         * @return Returns the application tag identifying a factor.
         */
        static ber::Tag Factor();

        /**
         * Returns the application tag identifying the online flag.
         * @return Returns the application tag identifying the online flag.
         */
        static ber::Tag IsOnline();

        /**
         * Returns the application tag identifying a formula.
         * @return Returns the application tag identifying a formula.
         */
        static ber::Tag Formula();

        /**
         * Returns the application tag identifying a step.
         * @return Returns the application tag identifying a step.
         */
        static ber::Tag Step();

        /**
         * Returns the application tag identifying a defalt value.
         * @return Returns the application tag identifying a default value.
         */
        static ber::Tag Default();

        /**
         * Returns the application tag identifying the command flag.
         * @return Returns the application tag identifying the command flag.
         */
        static ber::Tag IsCommand();

        /**
         * Returns the application tag identifying a stream identifier.
         * @return Returns the application tag identifying the stream identifier.
         */
        static ber::Tag StreamIdentifier();

        /**
         * Returns the application tag identifying the child collection of a node or parameter.
         * @return Returns the application tag identifying the child collection.
         */
        static ber::Tag Children();

        /**
         * Returns the application tag identifying a root element collection. 
         * @return Returns the application tag identifying a root.
         */
        static ber::Tag Root();

        /**
         * Returns the application tag that is used for a glow element when it is listed within
         * a sequence. The tag returned is ContextSpecific - 0.
         * @return Returns the application tag for listed glow elements.
         */
        static ber::Tag ElementDefault();

        /**
         * Returns the application tag identifying the contents of a node. The tag returned
         * is ContextSpecific - 0.
         * @return Returns the application tag identifying the node contents.
         */
        static ber::Tag NodeContents();

        /**
         * Returns the application tag identifying the contents of a parameter. The tag returned
         * is ContextSpecific - 0.
         * @return Returns the application tag identifying the parameter contents.
         */
        static ber::Tag ParameterContents();

        /**
         * Returns the application tag identifying a stream value. This tag is used inside the StreamEntry. 
         * The tag returned is ContextSpecific - 0.
         * @return Returns the application tag identifying a stream value.
         */
        static ber::Tag StreamValue();
    };
}
}

#ifdef LIBEMBER_HEADER_ONLY
#   include "impl/GlowTags.ipp"
#endif

#endif  // __LIBEMBER_GLOW_GLOWTAGS_HPP

