#ifndef __TINYEMBER_GADGET_DIRTYSTATE_H
#define __TINYEMBER_GADGET_DIRTYSTATE_H

namespace gadget
{
    template<typename FlagType>
    struct DirtyState;

    template<typename FlagType>
    bool operator ==(DirtyState<FlagType> const& x, DirtyState<FlagType> const& y);

    template<typename FlagType>
    bool operator !=(DirtyState<FlagType> const& x, DirtyState<FlagType> const& y);

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

            DirtyState(value_type state = value_type(0));

            template<typename OtherFlagType>
            DirtyState(DirtyState<OtherFlagType> const& other);

            bool isDirty() const;

            bool isSet(flag_type flag) const;

            void set(value_type flags);

            void clear();
        
            DirtyState const mask(DirtyState const& other) const;

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
