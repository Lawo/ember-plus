#ifndef STRINGSTATE_H
#define STRINGSTATE_H

#include "../State.h"

namespace gadgets { namespace states
{
    /**
     * String stae
     */
    class StringState : public State
    {
        friend class State;

    public:
        /**
         * Destructor
         */
        virtual ~StringState();

        /**
         * @see State
         */
        virtual State* clone(Entity const& owner) const;

        /**
         * @see State
         */
        virtual bool trySetValue(libember::glow::Value const& value);

        /**
         * Returns the current value
         * @return Current value
         */
        String const& value() const;

        /**
         * Returns the current max length of the string.
         */
        int maxLength() const;

        /**
         * Sets the value of this state
         * @param value Value to set
         */
        void setValue(String const& value);

        /**
         * Sets the maximum length for a string.
         * @param maxLength The new maximum length the string may have.
         * @note Set the length to 0 in order to allow unlimited string lengths.
         */
        void setMaxLength(int maxLength);

    private:
        /**
         * Constructor
         * @param owner The entity owning this state
         * @param writeable Initial writeable flag
         */
        StringState(Entity const& owner, bool writeable);

        /**
         * @see State
         */
        virtual void visitImpl(EntityTypeVisitor &visitor, Parameter &parameter);

        /**
         * @see State
         */
        virtual void visitImpl(EntityStateVisitor &visitor, Parameter const& parameter, PropertyState const& state) const;

    private:
        String m_value;
        int m_maxLength;
    };

}
}

#endif // STRINGSTATE_H
