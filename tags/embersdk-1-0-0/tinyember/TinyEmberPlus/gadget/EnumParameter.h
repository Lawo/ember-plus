#ifndef __TINYEMBER_GADGET_ENUMPARAMETER_H
#define __TINYEMBER_GADGET_ENUMPARAMETER_H

#include <algorithm>
#include <vector>
#include "Parameter.h"
#include "../Types.h"

namespace gadget 
{
    class ParameterFactory;

    class EnumParameter : public Parameter
    {
        friend class ParameterFactory;
        typedef std::vector<String> EnumContainer;
        public:
            typedef EnumContainer::value_type value_type;
            typedef EnumContainer::size_type size_type;
            typedef EnumContainer::iterator iterator;
            typedef EnumContainer::const_iterator const_iterator;

            template<typename InputIterator>
            void assign(InputIterator first, InputIterator last);

            void setIndex(size_type value);

            size_type size() const;

            bool empty() const;

            const_iterator begin() const;

            const_iterator end() const;

            size_type index() const;

            virtual void accept(ParameterTypeVisitorConst const& visitor) const;

            virtual void accept(ParameterTypeVisitor& visitor);

            virtual std::string toDisplayValue() const;

        private:
            EnumParameter(Node* parent, String const& identifier, int number);

            template<typename InputIterator>
            EnumParameter(Node* parent, String const& identifier, int number, InputIterator first, InputIterator last);

            EnumContainer m_enumeration;
            size_type m_index;
    };

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
