#ifndef __LIBEMBER_GLOW_GLOWTYPE_HPP
#define __LIBEMBER_GLOW_GLOWTYPE_HPP

#include "../ber/Tag.hpp"
#include "../ber/Type.hpp"

namespace libember { namespace glow
{
    /**
     * Contains the application defined types used by glow
     */
    class GlowType
    {
        public:
            enum _Domain
            {
                Parameter = 1,
                Command = 2,
                Node = 3,
                ElementCollection = 4,
                StreamEntry = 5,
                StreamCollection = 6,
                StringIntegerPair = 7,
                StringIntegerCollection = 8,
            };

            typedef unsigned int value_type;

        public:
            /**
             * Constructor 
             * @param type Glow Type.
             */
            GlowType(_Domain type);

            /**
             * Return the type identifier.
             * @return Returns the type identifier.
             */
            value_type value() const;

            /**
             * Returns the glow type as ber tag which is used when the object is being encoded.
             * @return The type tag representing this glow type.
             */
            ber::Tag toTypeTag() const;

        private:
            value_type m_value;
    };

    /**************************************************************************/
    /* Inline implementation                                                  */
    /**************************************************************************/

    inline GlowType::GlowType(_Domain type)
        : m_value(type)
    {}

    inline GlowType::value_type GlowType::value() const
    {
        return m_value;
    }

    inline ber::Tag GlowType::toTypeTag() const
    {
        return ber::make_tag(ber::Class::Application, m_value);
    }
}
}

#endif//__LIBEMBER_GLOW_GLOWTYPE_HPP
