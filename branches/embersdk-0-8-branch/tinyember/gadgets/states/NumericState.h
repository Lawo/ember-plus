#ifndef NUMERICSTATE_H
#define NUMERICSTATE_H

#include "../EntityStateVisitor.h"
#include "../EntityTypeVisitor.h"
#include "../State.h"
#include "../../Libember.h"

namespace gadgets { namespace states
{
    namespace meta
    {
        /**
         * Templated struct that helps to determine wether
         * a provided glow value can be converted to the type of a parameter state.
         */
        template<typename ValueType>
        struct GlowTypeToValueType
        {
            typedef ValueType value_type;

            /**
             * Evaluates if a glow value can be converted to the target type.
             * The generic implementation always returns false.
             * @param value Glow value
             * @return Returns true if the types are compatible.
             */
            static bool canConvert(libember::glow::Value const& value)
            {
                return false;
            }

            /**
             * Converts the provided value to the target type.
             * The generic implementation return 0.
             * @param value The glow value to convert.
             * @return Returns the converted value.
             */
            static value_type convert(libember::glow::Value const& value)
            {
                return value_type(0);
            }
        };

        /**
         * Specialization for double states.
         */
        template<>
        struct GlowTypeToValueType<double>
        {
            typedef double value_type;

            /**
             * @see GlowTypeToValueType<ValueType>
             */
            static bool canConvert(libember::glow::Value const& value)
            {
                return value.type() == libember::ber::Type::Real;
            }

            /**
             * @see GlowTypeToValueType<ValueType>
             */
            static value_type convert(libember::glow::Value const& value)
            {
                return value.toReal();
            }
        };

        /**
         * Specialization for integer states.
         */
        template<>
        struct GlowTypeToValueType<int>
        {
            typedef int value_type;

            /**
             * @see GlowTypeToValueType<ValueType>
             */
            static bool canConvert(libember::glow::Value const& value)
            {
                return value.type() == libember::ber::Type::Integer;
            }

            /**
             * @see GlowTypeToValueType<ValueType>
             */
            static value_type convert(libember::glow::Value const& value)
            {
                return value.toInteger();
            }
        };
    }


    /**
     * Numeric State implementation, may be used for integer and double parameter types
     */
    template<typename ValueType>
    class NumericState : public State
    {
        friend class State;

    public:
        typedef ValueType value_type;

    public:
        /**
         * @see State
         */
        virtual State* clone(const Entity &owner) const;

        /**
         * @see State
         */
        virtual bool trySetValue(libember::glow::Value const& value);

        /**
         * Changes the value
         * @param value The new value to set
         */
        void setValue(value_type value);

        /**
         * Sets the min value
         * @param The new minimum value
         */
        void setMin(value_type min);

        /**
         * Sets the max value
         * @return The new maximum value
         */
        void setMax(value_type max);

        /**
         * Sets the parameter's format string
         * @param format The new format string
         */
        void setFormat(String const& format);

        /**
         * Returns the current value
         * @return Returns the current value of the state
         */
        value_type value() const;

        /**
         * Returns the min value
         * @return Returns the minimum value
         */
        value_type min() const;

        /**
         * Returns the max value
         * @return Returns the current maximum value
         */
        value_type max() const;

        /**
         * Returns the format string
         * @return Returns the format string
         */
        String const& format() const;

    private:
        /**
         * Constructor
         * @param owner The entity owning this state
         * @param writeable Initial writeable state
         */
        NumericState(Entity const& owner, bool writeable);

        /**
         * @see State
         */
        virtual void visitImpl(EntityTypeVisitor& visitor, Parameter& parameter);

        /**
         * @see State
         */
        virtual void visitImpl(EntityStateVisitor& visitor, Parameter const& parameter, PropertyState const& state) const;

    private:
        value_type m_value;
        value_type m_min;
        value_type m_max;
        String m_format;
    };


    /**************************************************************************
     * Mandatory inline implementation                                        *
     **************************************************************************/

    template<typename ValueType>
    inline NumericState<ValueType>::NumericState(Entity const& owner, bool writeable)
        : State(owner, writeable, false)
        , m_value(0)
        , m_min(0)
        , m_max(100)
    {
    }

    template<typename ValueType> //virtual
    inline State* NumericState<ValueType>::clone(const Entity& owner) const
    {
        NumericState<value_type>* clone = new NumericState<value_type>(owner, isWriteable());
        clone->setWriteable(isWriteable());
        clone->setCommand(isCommand());
        clone->setMin(min());
        clone->setMax(max());
        clone->setValue(value());
        clone->setFormat(format());

        return clone;
    }

    template<typename ValueType>
    inline bool NumericState<ValueType>::trySetValue(libember::glow::Value const& value)
    {
        typedef meta::GlowTypeToValueType<ValueType> TypeConverter;

        if (TypeConverter::canConvert(value))
        {
            setValue(TypeConverter::convert(value));
            return true;
        }
        else
        {
            return false;
        }
    }

    template<typename ValueType>
    inline void NumericState<ValueType>::setFormat(String const& format)
    {
        if (m_format != format)
        {
            m_format = format;

            markDirty(Property::ValueFormat);
        }
    }

    template<typename ValueType>
    inline String const& NumericState<ValueType>::format() const
    {
        return m_format;
    }

    template<typename ValueType>
    inline void NumericState<ValueType>::setValue(value_type value)
    {
        if (m_value != value)
        {
            m_value = value;

            markDirty(Property::Value);
        }
    }

    template<typename ValueType>
    inline void NumericState<ValueType>::setMin(value_type min)
    {
        if (m_min != min)
        {
            m_min = min;

            markDirty(Property::ValueMin);
        }
    }

    template<typename ValueType>
    inline void NumericState<ValueType>::setMax(value_type max)
    {
        if (m_max != max)
        {
            m_max = max;

            markDirty(Property::ValueMax);
        }
    }

    template<typename ValueType>
    inline typename NumericState<ValueType>::value_type NumericState<ValueType>::value() const
    {
        return m_value;
    }

    template<typename ValueType>
    inline typename NumericState<ValueType>::value_type NumericState<ValueType>::min() const
    {
        return m_min;
    }

    template<typename ValueType>
    inline typename NumericState<ValueType>::value_type NumericState<ValueType>::max() const
    {
        return m_max;
    }

    template<typename ValueType> //virtual
    inline void NumericState<ValueType>::visitImpl(EntityTypeVisitor &visitor, Parameter &parameter)
    {
        visitor.visit(parameter, *this);
    }

    template<typename ValueType> //virtual
    inline void NumericState<ValueType>::visitImpl(EntityStateVisitor& visitor, Parameter const&, PropertyState const& state) const
    {
        if (state.isSet(Property::ValueFormat))
            visitor.addProperty(Property::ValueFormat, m_format);

        if (state.isSet(Property::ValueMin))
            visitor.addProperty(Property::ValueMin, m_min);

        if (state.isSet(Property::ValueMax))
            visitor.addProperty(Property::ValueMax, m_max);

        if (state.isSet(Property::Value))
            visitor.addProperty(Property::Value, m_value);
    }


    typedef NumericState<int> IntState;
    typedef NumericState<double> RealState;
}
}

#endif // NUMERICSTATE_H
