#ifndef DIRTYSTATE_H
#define DIRTYSTATE_H

namespace gadgets
{
    /**
     * This class manages bit field which is used to indicate a dirty state, where each bit has
     * a different meaning.
     */
    template<typename FlagType>
    struct DirtyState
    {
    public:
        typedef std::size_t value_type;
        typedef FlagType flag_type;

        /**
         * Constructor
         * @param state Initial state
         */
        DirtyState(value_type const state = 0);

        /**
         * Returns true if at least one bit is set
         */
        bool isDirty() const;

        /**
         * Checks for a specific flag
         * @param flag Flag to check
         * @return Returns true if the provided flag is set
         */
        bool isSet(flag_type flag) const;

        /**
         * Sets one or more flags
         * @param flags Flags to set
         */
        void set(value_type flags);

        /**
         * Clears all flags
         */
        void clear();

        /**
         * Returns a new dirty state which returns only the flags
         * that are set in both instances
         * @param other DirtyState to compare against
         * @return Returns the new state containing the flags set in both states
         */
        DirtyState const mask(DirtyState const& other) const;

        /**
         * Combines the flags of this instance with the one provided.
         * @param other DirtyState state to combine.
         * @return Returns the combined flags.
         */
        DirtyState const combine(DirtyState const& other) const;

    private:
        value_type m_state;
    };

    /**************************************************************************
     * Inline implementation                                                  *
     **************************************************************************/

    template<typename FlagType>
    inline DirtyState<FlagType>::DirtyState(value_type const state)
        : m_state(state)
    {
    }

    template<typename FlagType>
    inline bool DirtyState<FlagType>::isDirty() const
    {
        return m_state > 0;
    }

    template<typename FlagType>
    inline bool DirtyState<FlagType>::isSet(flag_type flag) const
    {
        return !!(m_state & flag);
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
    inline DirtyState<FlagType> const DirtyState<FlagType>::mask(DirtyState<FlagType> const& other) const
    {
        return DirtyState<FlagType>(m_state & other.m_state);
    }

    template<typename FlagType>
    inline DirtyState<FlagType> const DirtyState<FlagType>::combine(DirtyState const& other) const
    {
        return DirtyState<FlagType>(m_state | other.m_state);
    }
}


#endif // DIRTYSTATE_H
