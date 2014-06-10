/*
    libember -- C++ 03 implementation of the Ember+ Protocol
    Copyright (C) 2012-2014  L-S-B Broadcast Technologies GmbH

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

#ifndef __LIBEMBER_GLOW_GLOWCONNECTION_HPP
#define __LIBEMBER_GLOW_GLOWCONNECTION_HPP

#include "GlowContainer.hpp"
#include "ConnectionOperation.hpp"
#include "ConnectionDisposition.hpp"

namespace libember { namespace glow
{
    /** Forward declarations **/
    class GlowNodeFactory;

    /**
     * GlowConnection describes a connection on a matrix.
     */
    class LIBEMBER_API GlowConnection : public GlowContainer
    {
        friend class GlowNodeFactory;
        public:
            /**
             * Initializes a new instance of GlowConnection.
             * @param target The number of the reported target.
             */
            GlowConnection(int target);

            /**
             * Initializes a new instance of GlowConnection.
             * @param target The number of the reported target.
             * @param tag The application tag to set
             */
            GlowConnection(int target, ber::Tag const& tag);

            /**
             * Sets the connected sources as an ObjectIdentifier.
             * Each sub-id in the ObjectIdentifier contains the number of a connected source.
             * @param sources an ObjectIdentifier containing the sources to set.
             */
            void setSources(ber::ObjectIdentifier const& sources);

            /**
             * Sets the connection operation.
             * @param operation the operation to set.
             */
            void setOperation(ConnectionOperation const& operation);

            /**
             * Sets the connection disposition.
             * @param disposition the disposition to set.
             */
            void setDisposition(ConnectionDisposition const& disposition);

            /**
             * Returns the number of the reported target.
             * @return The number of the reported target.
             */
            int target() const;

            /**
             * Returns an ObjectIdentifier containing connected sources as subids.
             * @return an ObjectIdentifier containing connected sources as subids.
             */
            ber::ObjectIdentifier sources() const;

            /**
             * Returns the connection operation.
             * @return the connection operation.
             */
            ConnectionOperation operation() const;

            /**
             * Returns the connection disposition.
             * @return the connection disposition.
             */
            ConnectionDisposition disposition() const;

        private:
            /**
             * Initializes a new instance of GlowConnection.
             * Only called by GlowNodeFactory.
             * @param tag The application tag to set.
             */
            GlowConnection(ber::Tag const& tag);
    };
}
}

#ifdef LIBEMBER_HEADER_ONLY
#  include "impl/GlowConnection.ipp"
#endif

#endif  // __LIBEMBER_GLOW_GLOWCONNECTION_HPP
