#ifndef __TINYEMBER_GADGET_DIRTYSTATE_H
#define __TINYEMBER_GADGET_DIRTYSTATE_H

namespace gadget
{
    /** Forward declarations */
    template<typename FlagType>
    struct DirtyState;

    template<typename FlagType>
    bool operator ==(DirtyState<FlagType> const& x, DirtyState<FlagType> const& y);

    template<typename FlagType>
    bool operator !=(DirtyState<FlagType> const& x, DirtyState<FlagType> const& y);

    /**
     * This class stores a set of flags and allows to modify or query them bitwise.
     */
    template<typename FlagType>
    struct DirtyState
    {
        template<typename FlagType>
        friend bool operator ==(DirtyState<FlagType> const& x, DirtyState<FlagType> const& y);

        template<typename FlagType>
        friend bool operator !=(DirtyState<FlagType> const& x, DirtyState<FlagType> const& y);

        public:
            typedef std::size_t value_type;
            typedef FlagType flag_type;

            /** 
             * Initializes new DirtyState.
             * @param state The initial state.
             */
            DirtyState(value_type state = value_type(0));

            /**
             * Initializes a new DirtyState and copies the flags of the provided instance.
             * @param other The instance to copy the state from.
             */
            template<typename OtherFlagType>
            DirtyState(DirtyState<OtherFlagType> const& other);

            /**
             * Returns true when at least one bit is set.
             * @return true when at least one bit is set.
             */
            bool isDirty() const;

            /**
             * Tests whether a flag is set or not.
             * @param flag The flag to set.
             * @return true if the passed flag is set, otherwise false.
             */
            bool isSet(flag_type flag) const;

            /**
             * Sets the specified flag or set of flags.
             * @param flags The bits to add to this instance.
             */
            void set(value_type flags);

            /**
             * Resets the dirty state to zero.
             */
            void clear();
        
            /**
             * Returns a new DirtyState instance which only contains the bits
             * that were set in both instances.
             * @param other The second instance to perform the bitwise and operation with.
             * @return A new instance containing only the bits that were set in both instances.
             */
            DirtyState const mask(DirtyState const& other) const;

            /**
             * Returns a new DirtyState instance which contains all bits that
             * were set in at least one instance.
             * @param other The second instance to perform a bitwise or operation with.
             * @return A new instance containing all bits that were set in at least one instance.
             */
            DirtyState const join(DirtyState const& other) const;

        private:
            value_type m_state;
    };

    /**************************************************************************
     * inline implementation                                                  *
     **************************************************************************/

    template<typename FlagType>
    inline DirtyState<FlagType>::DirtyState(value_type state)
        : m_state(state)
    {
    }

    template<typename FlagType>
    template<typename OtherFlagType>
    inline DirtyState<FlagType>::DirtyState(DirtyState<OtherFlagType> const& other)
        : m_state(other.m_state)
    {
    }

    template<typename FlagType>
    inline bool DirtyState<FlagType>::isDirty() const
    {
        return m_state != 0;
    }

    template<typename FlagType>
    inline bool DirtyState<FlagType>::isSet(flag_type flag) const
    {
        return (m_state & flag) == flag;
    }

    template<typename FlagType>
    inline void DirtyState<FlagType>::set(value_type flags)
    {
        m_state |= flags;
    }

    template<typename FlagType>
    inline void DirtyState<FlagType>::clear()
    {
        m_state = 0;
    }
        
    template<typename FlagType>
    inline DirtyState<FlagType> const DirtyState<FlagType>::mask(DirtyState const& other) const
    {
        return DirtyState<FlagType>(m_state & other.m_state);
    }

    template<typename FlagType>
    inline DirtyState<FlagType> const DirtyState<FlagType>::join(DirtyState const& other) const
    {
        return DirtyState<FlagType>(m_state | other.m_state);
    }
    
    template<typename FlagType>
    inline bool operator ==(DirtyState<FlagType> const& x, DirtyState<FlagType> const& y)
    {
        return x.m_state == y.m_state;
    }

    template<typename FlagType>
    inline bool operator !=(DirtyState<FlagType> const& x, DirtyState<FlagType> const& y)
    {
        return x.m_state != y.m_state;
    }
}

#endif//__TINYEMBER_GADGET_DIRTYSTATE_H
