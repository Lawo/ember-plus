#ifndef __LIBEMBER_GLOW_GLOWPARAMETER_HPP
#define __LIBEMBER_GLOW_GLOWPARAMETER_HPP

#include <list>
#include <sstream>
#include "GlowContentElement.hpp"
#include "GlowElementCollection.hpp"
#include "MinMax.hpp"
#include "Value.hpp"
#include "util/ValueTypeToVariantLeaf.hpp"

namespace libember { namespace glow
{
    /** Forward declarations **/
    class GlowNodeFactory;

    /**
     * Class representing a parameter description.
     * The property that must always be set is the node's number. 
     * The methods to access a property of this object return a default value if a property doesn't exist.
     * To assure that the property exists, the contains method should be used.
     */
    class LIBEMBER_API GlowParameter : public GlowContentElement
    {
        friend class GlowNodeFactory;
        public:
            /**
             * List containing the entries of an enumeration.
             */
            typedef std::list<std::string> EnumerationList;

            /**
             * Initializes a parameter with the provided number.
             * @param number The parameter number.
             * @note The application tag will be set to ContextSpecific-0.
             */
            explicit GlowParameter(int number);

            /**
             * Constructor that initializes a parameter with the specified application tag.
             * @param number The parameter number.
             * @param tag Application tag.
             */
            GlowParameter(int number, ber::Tag const& tag);

            /**
             * Tests if the node contains the passed property.
             * @param property The property the look for.
             * @return Returns true if the property exists, false otherwise.
             */
            bool contains(GlowProperty const& property) const;

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
             * @param formula UPN term for the tx and rx computation, separated by a '\n' character.
             * @note Both terms must be provided in a single string. 
             */
            void setFormula(std::string const& formula);

            /**
             * Sets the enumeration property by concatenating the provided strings. The strings 
             * are separated by a '\n' character.
             * @first First element to use in the enumeration.
             * @last Points the the first element not being used.
             */
            template<typename InputIterator>
            void setEnumeration(InputIterator first, InputIterator last);

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
             * Sets the writeable flag.
             * @param isWriteable Determines wether this parameter can be modified or not.
             * @note When a data provider reports a parameter without this flag being set, a consumer
             *      shall consider the parameter as read-only by default.
             */
            void setIsWriteable(bool isWriteable);

            /**
             * Sets the command flag. If a parameter's IsCommand property is set to true, it should be displayed
             * as a button.
             * @param isCommand Command flag.
             */
            void setIsCommand(bool isCommand);

            /**
             * Sets the stream identifier. This property is only required when the parameter represents
             *      an audio level meter. This number must be unique within the device, since audio level
             *      meter values are being reported in a single sequence.
             * @param identifier Unique stream identifier.
             */
            void setStreamIdentifier(int identifier);

            /**
             * Returns a modifiable Element collection which may contain one or more commands that
             * shall be applied to this parameter.
             */
            GlowElementCollection* children();

            /**
             * Returns the number of this parameter.
             * @return The number of this parameter or -1 if not set.
             */
            int number() const;

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
             * @return The formula for RX and TX or "" if not set.
             */
            std::string formula() const;

            /**
             * Converts the enumeration property into a string list containing the single entries.
             * @return Returns a list containing all enumeration entries.
             */
            EnumerationList enumeration() const;

            /**
             * Returns the minimum property.
             * @return The minimum value allowed for the value of this parameter.
             * @note The type can be either double or integer.
             */
            MinMax minimum() const;

            /**
             * Returns the maximum property.
             * @return The maximum value allowed for the value of this parameter.
             * @note The type can be either doulbe or integer.
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
             * Returns the writeable flag.
             * @return The writeable flag.
             */
            bool isWriteable() const;

            /**
             * Returns the command flag.
             * @return The command flag.
             */
            bool isCommand() const;

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

        private:
            /**
             * This constructor initializes a parameter without the mandatory number.
             * It is only used by the NodeFactory since the number has not been decoded
             * when this container is being created.
             * @param tag Decoded application tag.
             */
            explicit GlowParameter(ber::Tag const& tag);
    };


    /******************************************************
     * Madatory inline implementation                     *
     ******************************************************/


    template<typename InputIterator>
    inline void GlowParameter::setEnumeration(InputIterator first, InputIterator last)
    {
        std::ostringstream stream;

        for(; first != last; )
        {
            stream << *first++;

            if (first != last)
                stream << "\n";
        }

        Contents& content = contents();
        content.insert(content.end(), util::ValueTypeToVariantLeaf::create(GlowTags::Enumeration(), stream.str()));
    }
}
}

#ifdef LIBEMBER_HEADER_ONLY
#   include "impl/GlowParameter.ipp"
#endif

#endif  // __LIBEMBER_GLOW_GLOWPARAMETER_HPP

