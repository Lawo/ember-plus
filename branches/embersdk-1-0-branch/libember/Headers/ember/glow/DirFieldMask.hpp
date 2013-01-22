/*
    libember -- C++ 03 implementation of the Ember+ Protocol
    Copyright (C) 2012  L-S-B Broadcast Technologies GmbH

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef __LIBEMBER_GLOW_DIRFIELDMASK_HPP
#define __LIBEMBER_GLOW_DIRFIELDMASK_HPP

namespace libember { namespace glow
{
    /**
     * A scoped enumeration type containing the symbolic names
     * for the optional flags of a GetDirectory request.
     */
    class DirFieldMask
    {
        public:
            enum _Domain
            {
                None = 0,
                Identifier = (1 << 0),
                Description = (1 << 1),
                Value = (1 << 2),

                All = 0xFFFFFFFF,
            };

            typedef int value_type;

        public:
            /**
             * Initializes a new instance of DirFieldFlags. 
             * @param flag The flag to initializes the instance with.
             */
            DirFieldMask(_Domain flag)
                : m_value(flag)
            {}

            /**
             * Initializes a new instance of DirFieldFlags. 
             * @param flags The flags to initializes the instance with.
             */
            explicit DirFieldMask(value_type flags)
                : m_value(flags)
            {}

            /**
             * Returns the mask.
             * @return The mask.
             */
            value_type value() const
            {
                return m_value;
            }

        private:
            value_type m_value;
    };
}
}

#endif  // __LIBEMBER_GLOW_DIRFIELDMASK_HPP
