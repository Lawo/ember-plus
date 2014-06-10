#ifndef __TINYEMBER_GADGET_ENUMPARAMETER_H
#define __TINYEMBER_GADGET_ENUMPARAMETER_H

#include <algorithm>
#include <vector>
#include "Parameter.h"
#include "../Types.h"

namespace gadget 
{
    /** Forward declaration */
    class ParameterFactory;

    /**
     * A parameter that is capable of storing an enumeration.
     */
    class EnumParameter : public Parameter
    {
        friend class ParameterFactory;
        typedef std::vector<String> EnumContainer;
        public:
            typedef EnumContainer::value_type value_type;
            typedef EnumContainer::size_type size_type;
            typedef EnumContainer::iterator iterator;
            typedef EnumContainer::const_iterator const_iterator;

            /**
             * Assigns a new collection of strings to this enumeration.
             * @param first A reference to the first string to copy.
             * @param last A reference to the end of the string collection.
             */
            template<typename InputIterator>
            void assign(InputIterator first, InputIterator last);

            /**
             * Changes the index of the selected item to the value passed.
             * @param value The new index to set.
             * @param forceNotification If set to true, an attached DirtyStateListener
             *      will receive an update notification, even if the index doesn't change.
             */
            void setIndex(size_type value, bool forceNotification = false);

            /**
             * Returns the number of enumeration entries.
             * @return The number of enumeration entries.
             */
            size_type size() const;

            /**
             * Returns true if the enumeration is empty.
             * @return true if the enumeration is empty.
             */
            bool empty() const;

            /**
             * Returns an iterator that points to the first element of the enumeration entries.
             * @return An iterator that points to the first element of the enumeration entries.
             */
            const_iterator begin() const;

            /**
             * Returns an iterator that points to an element one past the last entry of this enumeration.
             * @return An iterator that points to an element one past the last entry of this enumeration.
             */
            const_iterator end() const;

            /**
             * Returns the currently selected index of this enumeration.
             * @return The currently selected index of this enumeration.
             */
            size_type index() const;

            /** @see Parameter::accept() */
            virtual void accept(ParameterTypeVisitorConst const& visitor) const;

            /** @see Parameter::accept() */
            virtual void accept(ParameterTypeVisitor& visitor);

            /** @see Parameter::toDisplayValue() */
            virtual std::string toDisplayValue() const;

        protected:
            /**
             * Initializes a new EnumParameter
             * @param parent The node owning this parameter.
             * @param identifier The parameter's identifier.
             * @param number The number of this parameter.
             */
            EnumParameter(Node* parent, String const& identifier, int number);

            /**
             * Initializes a new EnumParameter with a set of enumeration entries.
             * @param parent The node owning this parameter.
             * @param identifier The parameter's identifier.
             * @param number The number of this parameter.
             * @param first An iterator pointing to the first entry to copy.
             */
            template<typename InputIterator>
            EnumParameter(Node* parent, String const& identifier, int number, InputIterator first, InputIterator last);

            /**
             * This method is invoked by the setValue method when a value changes or the forceNotification
             * flag is set to true. The default implementation is empty.
             * @param value The new value.
             * @param forceNotification Indicates whether an dirty event shall be signalled, even if
             *      the value didn't change.
             */
            virtual void setIndexImpl(size_type value, bool forceNotification);

        private:
            EnumContainer m_enumeration;
            size_type m_index;
    };

    /**************************************************************************
     * Inline implementation                                                  *
     **************************************************************************/

    template<typename InputIterator>
    inline EnumParameter::EnumParameter(Node* parent, String const& identifier, int number, InputIterator first, InputIterator last)
        : Parameter(ParameterType::Enum, parent, identifier, number)
        , m_enumeration(first, last)
        , m_index(0)
    {}

    template<typename InputIterator>
    inline void EnumParameter::assign(InputIterator first, InputIterator last)
    {
        auto const newSize = std::distance(first, last);
        auto const oldSize = size();
        auto const isEqualSequence = newSize == oldSize && std::equal(first, last, begin());
        if (isEqualSequence == false)
        {
            m_enumeration.assign(first, last);
            if (empty() == false)
            {
                if (m_index >= size())
                    m_index = 0;
            }

            markDirty(ParameterField::Value | ParameterField::ValueEnumeration, true);
        }
    }
}

#endif//__TINYEMBER_GADGET_ENUMPARAMETER_H
