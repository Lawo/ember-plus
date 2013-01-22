#ifndef __LIBEMBER_GLOW_GLOWTAGS_HPP
#define __LIBEMBER_GLOW_GLOWTAGS_HPP

#include "../util/Api.hpp"
#include "NodeProperty.hpp"
#include "ParameterProperty.hpp"

namespace libember { namespace glow
{ 
    /**
     * Lists all tags that are used by glow to store properties of nodes, parameters and other object types.
     */
    struct LIBEMBER_API GlowTags
    {
        LIBEMBER_API struct Command
        {
            /**
             * Returns the context-specific tag identifying a command number.
             * @return Returns the context-specific tag identifying a command number.
             */
            static ber::Tag Number();
        };

        LIBEMBER_API struct Node 
        {
            /**
             * Returns the context-specific tag identifying a number.
             * @return Returns the context-specific tag identifying a number.
             */
            static ber::Tag Number(); 

            /**
             * Returns the context-specific tag identifying the contents of a node. 
             * @return Returns the context-specific tag identifying the node contents.
             */
            static ber::Tag Contents();

            /**
             * Returns the context-specific tag identifying an identifier.
             * @return Returns the context-specific tag identifying an identifier.
             */
            static ber::Tag Identifier();

            /**
             * Returns the context-specific tag identifying a number.
             * @return Returns the context-specific tag identifying a number.
             */
            static ber::Tag Description();

            /**
             * Returns the context-specific tag identifying the collection containing the child elements.
             * @return Returns the context-specific tag identifying the collection containing the child elements.
             */
            static ber::Tag Children();

            /**
             * Returns the context-specific tag identifying the root flag.
             * @return Returns the context-specific tag identifying the root flag.
             */
            static ber::Tag IsRoot();
        };

        LIBEMBER_API struct Parameter
        {
            /**
             * Returns the context-specific tag identifying a number.
             * @return Returns the context-specific tag identifying a number.
             */
            static ber::Tag Number(); 

            /**
             * Returns the context-specific tag identifying the contents of a parameter. 
             * @return Returns the context-specific tag identifying the parameter contents.
             */
            static ber::Tag Contents();

            /**
             * Returns the context-specific tag identifying an identifier.
             * @return Returns the context-specific tag identifying an identifier.
             */
            static ber::Tag Identifier();

            /**
             * Returns the context-specific tag identifying a number.
             * @return Returns the context-specific tag identifying a number.
             */
            static ber::Tag Description();

            /**
             * Returns the context-specific tag identifying a value.
             * @return Returns the context-specific tag identifying a value.
             */
            static ber::Tag Value();

            /**
             * Returns the context-specific tag identifying a minimum.
             * @return Returns the context-specific tag identifying a minimum.
             */
            static ber::Tag Minimum();

            /**
             * Returns the context-specific tag identifying a maximum.
             * @return Returns the context-specific tag identifying a maximum.
             */
            static ber::Tag Maximum();

            /**
             * Returns the context-specific tag identifying the writeable flag.
             * @return Returns the context-specific tag identifying the writeable flag.
             */
            static ber::Tag IsWriteable();

            /**
             * Returns the context-specific tag identifying a format string.
             * @return Returns the context-specific tag identifying a format string.
             */
            static ber::Tag Format();

            /**
             * Returns the context-specific tag identifying an enumeration.
             * @return Returns the context-specific tag identifying an enumeration.
             */
            static ber::Tag Enumeration();

            /**
             * Returns the context-specific tag identifying a factor.
             * @return Returns the context-specific tag identifying a factor.
             */
            static ber::Tag Factor();

            /**
             * Returns the context-specific tag identifying the online flag.
             * @return Returns the context-specific tag identifying the online flag.
             */
            static ber::Tag IsOnline();

            /**
             * Returns the context-specific tag identifying a formula.
             * @return Returns the context-specific tag identifying a formula.
             */
            static ber::Tag Formula();

            /**
             * Returns the context-specific tag identifying a step.
             * @return Returns the context-specific tag identifying a step.
             */
            static ber::Tag Step();

            /**
             * Returns the context-specific tag identifying a defalt value.
             * @return Returns the context-specific tag identifying a default value.
             */
            static ber::Tag Default();

            /**
             * Returns the context-specific tag identifying the command flag.
             * @return Returns the context-specific tag identifying the command flag.
             */
            static ber::Tag IsCommand();

            /**
             * Returns the context-specific tag identifying a stream identifier.
             * @return Returns the context-specific tag identifying the stream identifier.
             */
            static ber::Tag StreamIdentifier();

            /**
             * Returns the context-specific tag identifying the child collection of a node or parameter.
             * @return Returns the context-specific tag identifying the child collection.
             */
            static ber::Tag Children();

            /**
             * Returns the context-specific tag identifying an enumeration map. The map has a higher priority
             * than the enumeration string. It contains a list of pairs, where each pair has an index and a string value.
             * @return Returns the context-specific tag identifying an enumeration map.
             */
            static ber::Tag EnumMap();
        };
        
        LIBEMBER_API struct StreamEntry
        {
            /**
             * Returns the context-specific tag identifying a stream identifier within a stream entry.
             * @return Returns the context-specific tag of a stream identifier
             */
            static ber::Tag StreamIdentifier();

            /**
             * Returns the context-specific tag identifying a stream value withing a stream entry.
             * The tag returned is ContextSpecific - 0.
             * @return Returns the context-specific tag identifying a stream value.
             */
            static ber::Tag StreamValue();
        };

        LIBEMBER_API struct StringIntegerPair
        {
            /**
             * Returns the context-specific tag identifying a String-Integer pair's name
             * Returns the context-specific tag identifying a String-Integer pair's name
             */
            static ber::Tag Name();

            /**
             * Returns the context-specific tag identifying a String-Integer pair's value
             * Returns the context-specific tag identifying a String-Integer pair's value
             */
            static ber::Tag Value();
        };

        /**
         * Returns the tag identifying a root element collection. 
         * @return Returns the tag identifying a root.
         */
        static ber::Tag Root();

        /**
         * Returns the tag that is used for a glow element when it is listed within
         * a sequence. The tag returned is ContextSpecific - 0.
         * @return Returns the tag for listed glow elements.
         */
        static ber::Tag ElementDefault();
    };
}
}

#ifdef LIBEMBER_HEADER_ONLY
#   include "impl/GlowTags.ipp"
#endif

#endif  // __LIBEMBER_GLOW_GLOWTAGS_HPP

