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

#ifndef __LIBEMBER_GLOW_GLOWPARAMETERBASE_HPP
#define __LIBEMBER_GLOW_GLOWPARAMETERBASE_HPP

#include <list>
#include <sstream>
#include "Access.hpp"
#include "Enumeration.hpp"
#include "Formula.hpp"
#include "GlowContentElement.hpp"
#include "GlowElementCollection.hpp"
#include "GlowStreamDescriptor.hpp"
#include "GlowStringIntegerCollection.hpp"
#include "MinMax.hpp"
#include "ParameterProperty.hpp"
#include "ParameterType.hpp"
#include "Value.hpp"
#include "util/ValueConverter.hpp"

namespace libember { namespace glow
{
    /**
     * Class representing a base parameter description.
     * The property that must always be set is the node's number. 
     * The methods to access a property of this object return a default value if a property doesn't exist.
     * To assure that the property exists, the contains method should be used.
     */
    class LIBEMBER_API GlowParameterBase : public GlowContentElement
    {
        public:
            /**
             * Tests if the node contains the passed property.
             * @param property The property the look for.
             * @return Returns true if the property exists, false otherwise.
             */
            bool contains(ParameterProperty const& property) const;

            /**
             * Sets the description of this parameter.
             * @param description Description string.
             */
            void setDescription(std::string const& description);

            /**
             * Sets the identifier of this parameter
             * @param identifier Identifier of this parameter.
             */
            void setIdentifier(std::string const& identifier);

            /**
             * Sets the formula.
             * @param providerToConsumer The term to compute the device value from a display value.
             * @param consumerToProvider The term to compute the display value from a device value.
             */
            void setFormula(std::string const& providerToConsumer, std::string const& consumerToProvider);

            /**
             * Sets the C-Style format string
             * @param format The C-Style format string for this parameter
             */
            void setFormat(std::string const& format);

            /**
             * Sets the enumeration property by concatenating the provided strings. The strings 
             * are separated by a '\n' character.
             * @first First element to use in the enumeration.
             * @last Points the the first element not being used.
             */
            template<typename InputIterator>
            void setEnumeration(InputIterator first, InputIterator last);

            /**
             * Sets the enumeration property.
             * @param enumeration The completed enumeration string containing all entries separated
             *      by the '\n' character.
             */
            void setEnumeration(std::string const& enumeration);

            /**
             * Sets the enumeration map. The map is used if a provider doesn't use consecutive 
             * indices for an enumerated parameter. If a map is set, the default enumeration property
             * must be ignored.
             * @param first The first enumeration entry containing the name-value pair.
             * @param last The end of the enumeration entry buffer.
             * @note The InputIterator must contain a pair of type std::tuple<std::string, int>, where
             *      the string is the enumeration name and the int the enumeration value to send
             *      to the provider.
             */
            template<typename InputIterator>
            void setEnumerationMap(InputIterator first, InputIterator last);

            /**
             * Sets the smallest value allowed for this parameter.
             * @param minimum Smallest value allowed, as double.
             * @note The minimum should be provided in the same type like the value.
             */
            void setMinimum(double minimum);

            /**
             * Sets the smallest value allowed for this parameter.
             * @param minimum Smallest value allowed, as integer.
             * @note The minimum should be provided in the same type like the value.
             */
            void setMinimum(int minimum);

            /**
             * Sets the biggest value allowed for this parameter.
             * @param maximum Biggest value allowed, as double.
             * @note The maximum should be provided in the same type like the value.
             */
            void setMaximum(double maximum);

            /**
             * Sets the biggest value allowed for this parameter.
             * @param maximum Biggest value allowed, as integer.
             * @note The maximum should be provided in the same type like the value.
             */
            void setMaximum(int maximum);

            /**
             * Sets the factor the value has to be divded by when being displayed.
             * @param factor The parameter's factor, must not be 0.
             */
            void setFactor(int factor);

            /**
             * Sets the step value which is the dividen the parameter value has to be divided
             * by when being displayed.
             * @param step The step of this parameter
             */
            void setStep(double step);

            /**
             * Sets the default value.
             * @param value default value.
             */
            void setDefault(double value);

            /**
             * Sets the default value.
             * @param value default value.
             */
            void setDefault(int value);

            /**
             * Sets the default value.
             * @param value default value.
             */
            void setDefault(std::string const& value);

            /**
             * Sets the value of this parameter.
             * @param value Parameter value, as double.
             */
            void setValue(double value);

            /**
             * Sets the value of this parameter.
             * @param value Parameter value, as integer.
             * @note The integer type must also be used when the parameter contains an enumeration.
             *      The value then contains the index of the enumeration entries.
             */
            void setValue(int value);

            /**
             * Sets the value of this parameter.
             * @param value Parameter value, as string.
             */
            void setValue(std::string const& value);

            /**
             * Sets the way the parameter can be accessed.
             * @param access Determines wether this parameter can be modified or not.
             * @note When a data provider reports a parameter without this flag being set, a consumer
             *      shall consider the parameter as read-only by default.
             */
            void setAccess(Access const& access);

            /**
             * Sets the parameter type.
             * @param type The parameter's value type.
             */
            void setType(ParameterType const& type);

            /**
             * Sets the stream identifier. This property is only required when the parameter represents
             *      an audio level meter. This number must be unique within the device, since audio level
             *      meter values are being reported in a single sequence.
             * @param identifier Unique stream identifier.
             */
            void setStreamIdentifier(int identifier);

            /**
             * Inserts a StreamDescriptor with the provided format and offset.
             * @note When providing a StreamDescriptor, the StreamIdentifier must also be set.
             * @param format Stream format.
             * @param offset The stream value offset within the StreamEntry.
             */
            void setStreamDescriptor(StreamFormat const& format, int offset);

            /**
             * Returns a modifiable Element collection which may contain one or more commands that
             * shall be applied to this parameter.
             */
            GlowElementCollection* children();

            /**
             * Returns the description of this parameter.
             * @return The description of this parameter or "" if not set.
             */
            std::string description() const;

            /**
             * Returns the identifier string.
             * @return The identifier or "" if not set.
             */
            std::string identifier() const;

            /**
             * Returns the formula.
             * @return The formula which contains the terms required to compute the display and device value(s).
             */
            Formula formula() const;

            /**
             * Returns the format string
             * @return The format string of this parameter or "" if not set.
             */
            std::string format() const;

            /**
             * Converts the enumeration property into a string list containing the single entries.
             * @return Returns a list containing all enumeration entries.
             */
            Enumeration enumeration() const;

            /**
             * Returns the enumeration map, which is a list of (name, value) tuples
             * @return The numeration map
             */
            Enumeration enumerationMap() const;

            /**
             * Returns the minimum property.
             * @return The minimum value allowed for the value of this parameter.
             * @note The type can be either double or integer.
             */
            MinMax minimum() const;

            /**
             * Returns the maximum property.
             * @return The maximum value allowed for the value of this parameter.
             * @note The type can be either double or integer.
             */
            MinMax maximum() const;

            /**
             * Returns the factor of this parameter.
             * @return The factor.
             */
            int factor() const;

            /**
             * Returns the default value.
             * @return The default value.
             * @note The value can be a double, string or integer.
             */
            Value defaultValue() const;

            /**
             * Returns the parameter's value.
             * @return The parameter's value.
             * @note The value can be a double, string or integer.
             */
            Value value() const;

            /**
             * Returns the parameter's step or 1.0 if the property is not set.
             * @return The parameter's step.
             */
            double step() const;

            /**
             * Returns whether the parameter is writeable or not.
             * @return True if the parameter is writeable.
             */
            bool isWriteable() const;

            /**
             * Returns the access modifier of this parameter.
             * @return The access modifier of this parameter.
             */
            Access access() const;

            /**
             * Returns the parameter's value type.
             * @return The parameter's value type.
             */
            ParameterType type() const;

            /**
             * Returns the unique stream identifier.
             * @return The stream identifier.
             */
            int streamIdentifier() const;

            /**
             * Returns a immutable element collection containing the children of this parameter.
             * @return The element collection which may contain commands.
             */
            GlowElementCollection const* children() const;

            /**
             * Returns the StreamDescriptor, if available.
             * @return The StreamDescriptor object, if set. Otherwise, null is being returned.
             */
            GlowStreamDescriptor const* streamDescriptor() const;

        protected:
            /**
             * This constructor initializes a parameter without the mandatory number.
             * It is only used by the NodeFactory since the number has not been decoded
             * when this container is being created.
             * @param type Parameter type.
             * @param tag Application tag.
             * @param contentsTag The tag identifying the contents set.
             * @param childrenTag The tag identifying the children.
             */
            explicit GlowParameterBase(GlowType const& type, ber::Tag const& tag, ber::Tag const& contentsTag, ber::Tag const& childrenTag);
            
        private:
            ber::Tag m_childrenTag;
    };


    /******************************************************
     * Mandatory inline implementation                     *
     ******************************************************/


    template<typename InputIterator>
    inline void GlowParameterBase::setEnumeration(InputIterator first, InputIterator last)
    {
        std::ostringstream stream;

        for(/** Nothing */; first != last; /** Nothing */)
        {
            stream << *first++;

            if (first != last)
                stream << "\n";
        }

        setEnumeration(stream.str());
    }

    template<typename InputIterator>
    inline void GlowParameterBase::setEnumerationMap(InputIterator first, InputIterator last)
    {
        Contents& content = contents();
        Contents::iterator const where = content.end();
        ber::Tag const tag = GlowTags::ParameterContents::EnumMap();
        GlowStringIntegerCollection* enumeration = new GlowStringIntegerCollection(tag);

        for(/** Nothing */; first != last; ++first)
        {
            std::string const& name = first->first;
            int const& value = first->second;

            enumeration->insert(name, value);
        }

        content.insert(where, enumeration);
    }
}
}

#ifdef LIBEMBER_HEADER_ONLY
#   include "impl/GlowParameterBase.ipp"
#endif

#endif  // __LIBEMBER_GLOW_GLOWPARAMETERBASE_HPP
