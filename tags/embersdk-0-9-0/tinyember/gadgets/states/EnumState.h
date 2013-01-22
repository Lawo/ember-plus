#ifndef ENUMSTATE_H
#define ENUMSTATE_H

#include <iterator>
#include "../State.h"

namespace gadgets { namespace states
{
    /**
     * Enumeration State
     */
    class EnumState : public State
    {
        friend class State;

    public:
        typedef std::list<String> EnumList;
        typedef EnumList::value_type value_type;
        typedef EnumList::size_type index_type;
        typedef EnumList::const_iterator const_iterator;
        typedef EnumList::size_type size_type;

    public:
        /**
         * Destructor
         */
        virtual ~EnumState();

        /**
         * @see State
         */
        virtual State* clone(Entity const& owner) const;

        /**
         * @see State
         */
        virtual bool trySetValue(libember::glow::Value const& value);

        /**
         * Assigns new enumeration entries to this state
         * @param first First item to copy
         * @param last Indicates the end of the iterator stream
         */
        template<typename InputIterator>
        void assign(InputIterator first, InputIterator last);

        /**
         * Sets the current enumeration index
         * @param index New index
         */
        void setIndex(index_type index);

        /**
         * Returns the current enumeration
         */
        index_type index() const;

        /**
         * Returns the number of enumeration entries
         */
        size_type size() const;

        /**
         * Returns the first item of the enumeration
         */
        const_iterator begin() const;

        /**
         * Returns the last item of the enumeration, which is the first entry not containing a valid value
         */
        const_iterator end() const;

    private:
        /**
         * Constructor
         * @param owner The entity owning this state
         * @param writeable Initial writeable flag
         */
        EnumState(Entity const& owner, bool writeable);

        /**
         * @see State
         */
        virtual void visitImpl(EntityTypeVisitor& visitor, Parameter &parameter);

        /**
         * @see State
         */
        virtual void visitImpl(EntityStateVisitor& visitor, Parameter const& parameter, PropertyState const& state) const;

    private:
        EnumList m_entries;
        index_type m_index;

    };

    /**************************************************************************
     * Inline implementation                                                  *
     **************************************************************************/

    inline EnumState::const_iterator EnumState::begin() const
    {
        return m_entries.begin();
    }

    inline EnumState::const_iterator EnumState::end() const
    {
        return m_entries.end();
    }

    inline EnumState::size_type EnumState::size() const
    {
        return m_entries.size();
    }

    template<typename InputIterator>
    inline void EnumState::assign(InputIterator first, InputIterator last)
    {
        std::size_t const otherLength = std::distance(first, last);
        std::size_t const thisLength = size();
        bool const sequenceEquals = otherLength == thisLength && std::equal(first, last, begin());

        if (sequenceEquals == false)
        {
            m_entries.clear();
            m_entries.assign(first, last);

            if (m_index >= otherLength)
            {
                m_index = 0;

                markDirty(Property::ValueEnumeration | Property::Value);
            }
            else
            {
                markDirty(Property::ValueEnumeration);
            }

        }
    }
}
}

#endif // ENUMSTATE_H
