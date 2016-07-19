/*
    libember -- C++ 03 implementation of the Ember+ Protocol

    Copyright (C) 2012-2016 Lawo GmbH (http://www.lawo.com).
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef __LIBEMBER_GLOW_GLOWPARAMETER_HPP
#define __LIBEMBER_GLOW_GLOWPARAMETER_HPP

#include "GlowParameterBase.hpp"

namespace libember { namespace glow
{
    /** Forward declarations **/
    class GlowNodeFactory;
    class GlowNodeBase;

    /**
     * Class representing a parameter description.
     * The property that must always be set is the node's number. 
     * The methods to access a property of this object return a default value if a property doesn't exist.
     * To assure that the property exists, the contains method should be used.
     */
    class LIBEMBER_API GlowParameter : public GlowParameterBase
    {
        friend class GlowNodeFactory;
        public:
            /**
             * Initializes a parameter with the provided number.
             * @param number The parameter number.
             * @note The application tag will be set to ContextSpecific-0.
             */
            explicit GlowParameter(int number);

            /**
             * Initializes a new GlowParameter and adds it as child node to children collection
             * of the provided parent.
             * @param parent The parent node of this parameter.
             * @param number The parameter number.
             */
            GlowParameter(GlowNodeBase* parent, int number);

            /**
             * Constructor that initializes a parameter with the specified application tag.
             * @param number The parameter number.
             * @param tag Application tag.
             */
            GlowParameter(int number, ber::Tag const& tag);

            /**
             * Returns the number of this parameter.
             * @return The number of this parameter or -1 if not set.
             */
            int number() const;

        protected:
            /** 
             * @see Container::insertImpl() 
             * @remarks This override resets the cached number.
             */
            virtual iterator insertImpl(iterator const& where, Node* child);

            /** 
             * @see Container::eraseImpl() 
             * @remarks This override resets the cached number.
             */
            virtual void eraseImpl(iterator const& first, iterator const& last);

        private:
            /**
             * This constructor initializes a parameter without the mandatory number.
             * It is only used by the NodeFactory since the number has not been decoded
             * when this container is being created.
             * @param tag Decoded application tag.
             */
            explicit GlowParameter(ber::Tag const& tag);

        private:
            mutable int m_cachedNumber;
    };
}
}

#ifdef LIBEMBER_HEADER_ONLY
#  include "impl/GlowParameter.ipp"
#endif

#endif  // __LIBEMBER_GLOW_GLOWPARAMETER_HPP
