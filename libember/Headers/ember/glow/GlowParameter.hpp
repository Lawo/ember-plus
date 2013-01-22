#ifndef __LIBEMBER_GLOW_GLOWPARAMETER_HPP
#define __LIBEMBER_GLOW_GLOWPARAMETER_HPP

#include <list>
#include <sstream>
#include "GlowContentElement.hpp"
#include "GlowElementCollection.hpp"
#include "GlowStringIntegerCollection.hpp"
#include "MinMax.hpp"
#include "ParameterProperty.hpp"
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
             * List containing the string of an enumeration.
             */
            typedef std::list<std::string> EnumerationList;

            /**
             * List containing enumeration entries
             */
            typedef std::list<std::pair<std::string, int> > EnumerationEntryList;

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
             * @param formula UPN term for the tx and rx computation, separated by a '\n' character.
             * @note Both terms must be provided in a single string. 
             */
            void setFormula(std::string const& formula);

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
             * @note The InputIterator must contain a pair of type std::type<std::string, int>, where
             *      the string is the enumeration name and the int the enumeration value to send
             *      to the provider.
             */
            template<typename InputIterator>
            void setEnumMap(InputIterator first, InputIterator last);

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
             * Returns the format string
             * @return The format string of this parameter or "" if not set.
             */
            std::string format() const;

            /**
             * Converts the enumeration property into a string list containing the single entries.
             * @return Returns a list containing all enumeration entries.
             */
            EnumerationList enumeration() const;

            /**
             * Returns the enumeration map, which is a list of (name, value) tuples
             * @return The numeration map
             */
            EnumerationEntryList enumMap() const;

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
             * Returns the parameter's step or 1.0 if the property is not set.
             * @return The parameter's step.
             */
            double step() const;

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

            /**
             * Creates a VariantLeaf from the provided value and appends
             * it at the end of the collection.
             * @param tag Application tag of the value to insert.
             * @param value The value to insert.
             */
            template<typename ValueType>
            void append(ber::Tag const& tag, ValueType value);

            /**
             * Traverses the contents set and searches for a node with the provided tag.
             * If the node exists, its content will then be extracted and value is being
             * returned. 
             * This only works when the node is a VariantLeaf and its value type matches
             * the type passed to this method.
             * @param tag The application tag of the node.
             * @param defaultValue The default value. This parameter is used to resolve the
             *      desired value type at compile.
             * @return Returns the leaf's value if it exists or the provided default.
             */
            template<typename ValueType>
            ValueType getContentValue(ber::Tag const& tag, ValueType const& defaultValue) const;
    };


    /******************************************************
     * Madatory inline implementation                     *
     ******************************************************/


    template<typename InputIterator>
    inline void GlowParameter::setEnumeration(InputIterator first, InputIterator last)
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
    inline void GlowParameter::setEnumMap(InputIterator first, InputIterator last)
    {
        Contents& content = contents();
        Contents::iterator const where = content.end();
        ber::Tag const tag = GlowTags::Parameter::EnumMap();
        GlowStringIntegerCollection* enumCollection = new GlowStringIntegerCollection(tag);

        for(/** Nothing */; first != last; ++first)
        {
            std::string const& name = first->first;
            int const& value = first->second;

            enumCollection->insert(name, value);
        }

        content.insert(where, enumCollection);
    }

    template<typename ValueType>
    inline void GlowParameter::append(ber::Tag const& tag, ValueType value)
    {
        Contents& content = contents();
        Contents::iterator const where = content.end();
        dom::VariantLeaf *const leaf = util::ValueTypeToVariantLeaf::create(tag, value);

        content.insert(where, leaf);
    }

    template<typename ValueType>
    inline ValueType GlowParameter::getContentValue(ber::Tag const& tag, ValueType const& defaultValue) const
    {
        Contents const& content = contents();
        Contents::const_iterator const first = content.begin();
        Contents::const_iterator const last = content.end();
        dom::VariantLeaf const* leaf = find<dom::VariantLeaf>(first, last, tag);

        return util::LeafToValueType::as(leaf, defaultValue);
    }
}
}

#ifdef LIBEMBER_HEADER_ONLY
#   include "impl/GlowParameter.ipp"
#endif

#endif  // __LIBEMBER_GLOW_GLOWPARAMETER_HPP

