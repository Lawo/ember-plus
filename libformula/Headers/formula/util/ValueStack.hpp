#ifndef __LIBFORMULA_UTIL_VALUESTACK_HPP
#define __LIBFORMULA_UTIL_VALUESTACK_HPP

#include <stack>
#include "../Types.hpp"

namespace libformula { namespace util
{
    /** Forward declarations */
    struct ValueStackItem;

    ValueStackItem operator+(ValueStackItem const& x, ValueStackItem const& y);
    ValueStackItem operator-(ValueStackItem const& x, ValueStackItem const& y);
    ValueStackItem operator*(ValueStackItem const& x, ValueStackItem const& y);
    ValueStackItem operator/(ValueStackItem const& x, ValueStackItem const& y);
    ValueStackItem operator%(ValueStackItem const& x, ValueStackItem const& y);
    ValueStackItem operator-(ValueStackItem const& x);


    /**
     * Identifies the type of a stack item, which may either be long or real.
     */
    struct ValueStackItemType
    {
        public:
            /**
             * Enumeration containing the available types.
             */
            enum _Domain
            {
                Long = 1,
                Real,
            };

            typedef unsigned int value_type;

            /**
             * Initializes a new ValueStackItemType with the specified value.
             * @param type The type to initialize this instance with.
             */
            ValueStackItemType(_Domain type)
                : m_value(type)
            {}

            /**
             * Returns the type value.
             * @return The type value.
             */
            value_type value() const
            {
                return m_value;
            }

        private:
            value_type m_value;
    };


    /**
     * Represents an item that can be pushed onto the value stack. It may be of integer
     * or real type, and converted from one to another.
     */ 
    struct ValueStackItem
    {
        /**
         * Struct used to convert a stack item of any type supported into either a real or a long value.
         */
        template<int ItemType, typename DestType>
        struct ItemCast
        {
        };

        /**
         * Long to long conversion.
         */
        template<>
        struct ItemCast<ValueStackItemType::Long, long_type>
        {
            static long_type as(ValueStackItem const& item)
            {
                return item.m_long;
            }
        };

        /**
         * Long to real conversion
         */
        template<>
        struct ItemCast<ValueStackItemType::Long, real_type>
        {
            static real_type as(ValueStackItem const& item)
            {
                return static_cast<real_type>(item.m_long);
            }
        };

        /**
         * Real to long conversion
         */
        template<>
        struct ItemCast<ValueStackItemType::Real, long_type>
        {
            static long_type as(ValueStackItem const& item)
            {
                return static_cast<long_type>(item.m_real);
            }
        };

        /**
         * Real to real conversion
         */
        template<>
        struct ItemCast<ValueStackItemType::Real, real_type>
        {
            static real_type as(ValueStackItem const& item)
            {
                return item.m_real;
            }
        };

        public:
            /**
             * Initializes a new ValueStackItem with the specified long value.
             * @param value The value to initialize the item with.
             */
            ValueStackItem(long_type value)
                : m_type(ValueStackItemType::Long)
                , m_long(value)
            {}

            /**
             * Initializes a new ValueStackItem with the specified real value.
             * @param value The value to initialize the item with.
             */
            ValueStackItem(real_type value)
                : m_type(ValueStackItemType::Real)
                , m_real(value)
            {}

            /**
             * Returns the type of this value.
             * @return The type of this value.
             */
            ValueStackItemType const& type() const
            {
                return m_type;
            }

            /**
             * Converts the value to the type provided as template parameter.
             * @return Returns the converted value.
             */
            template<typename DestType>
            DestType toValueType() const
            {
                auto const type = m_type.value();
                if (type == ValueStackItemType::Long)
                    return as<ValueStackItemType::Long, DestType>();
                else if (type == ValueStackItemType::Real)
                    return as<ValueStackItemType::Real, DestType>();
                else
                    return DestType(0);
            }

        private:
            /**
             * Converts the value to the type provided by the template parameter.
             * The first template parameter determines the type of the stack item value.
             * @return Returns the converted value.
             */ 
            template<int ItemType, typename DestType>
            DestType as() const
            {
                return ItemCast<ItemType, DestType>::as(*this);
            }

        private:
            ValueStackItemType m_type;
            union
            {
                long_type m_long;
                real_type m_real;
            };
    };

    /**
     * The value stack can store values of real and long types. Additionaly, it may
     * convert a long value to a real value and vice versa.
     */
    class ValueStack
    {
        typedef std::stack<ValueStackItem> ValueStackItemContainer;
        public:
            typedef ValueStackItemContainer::value_type value_type;
            typedef ValueStackItemContainer::const_reference const_reference;

            /**
             * Removes the current item from the stack and returns it. This method
             * must not be called when the stack is empty, since there are no security
             * checks made.
             * @return Returns the ValueStackItem that has been removed from the stack.
             */
            value_type pop();

            /**
             * Removes the current item from the stack and converts it the the type specified
             * by the template parameter. This method
             * must not be called when the stack is empty, since there are no security
             * checks made.
             * @return Returns the converted value that has been removed from the stack.
             */
            template<typename ValueType>
            ValueType pop();

            /**
             * Returns a reference to the item that is currently on top of the stack. This method
             * must not be called when the stack is empty, since there are no security
             * checks made.
             * @return Returns the iterator that points the the item on top of the stack.
             */
            const_reference top();

            /**
             * Pushes the provided value onto the stack.
             * @param value The value to push onto the stack.
             */
            void push(long_type value);

            /**
             * Pushes the provided value onto the stack.
             * @param value The value to push onto the stack.
             */
            void push(real_type value);

            /**
             * Pushes the provided value onto the stack.
             * @param value The value to push onto the stack.
             */
            void push(value_type const& value);

            /**
             * Removes all items from the stack.
             */
            void clear();

            /**
             * Returns true if the stack is empty, otherwise false.
             * @return true if the stack is empty, otherwise false.
             */
            bool empty() const;

            /**
             * Returns true if the stack contains at least one item.
             * @return True if the stack is not empty.
             */
            bool any() const;

        private:
            ValueStackItemContainer m_stack;
    };

    /**************************************************************************
     * Inline implementation                                                  *
     **************************************************************************/

    inline ValueStack::value_type ValueStack::pop()
    {
        auto const top = m_stack.top();
        m_stack.pop();
        return top;
    }

    template<typename ValueType>
    inline ValueType ValueStack::pop()
    {
        auto const item = pop();
        return item.toValueType<ValueType>();
    }

    inline ValueStack::const_reference ValueStack::top()
    {
        return m_stack.top();
    }

    inline void ValueStack::push(value_type const& value)
    {
        m_stack.push(value);
    }

    inline void ValueStack::push(long_type value)
    {
        m_stack.push(value);
    }

    inline void ValueStack::push(real_type value)
    {
        m_stack.push(value);
    }

    inline void ValueStack::clear()
    {
        while(m_stack.empty() == false)
            m_stack.pop();
    }

    inline bool ValueStack::empty() const
    {
        return m_stack.empty();
    }

    inline bool ValueStack::any() const
    {
        return m_stack.empty() == false;
    }


    namespace detail
    {
        /**
         * Provides stack item based mathematical operations.
         */
        struct ValueStackItemOperation
        {
            /**
             * Evaluates whether an operation between the two operands should be performed
             * as real or long operation.
             * @param x First operand.
             * @param y Second operand.
             * @return Returns true, if at least one of the two operands is of type real.
             */
            inline static bool isreal(ValueStackItem const& x, ValueStackItem const& y)
            {
                auto const result = x.type().value() | y.type().value();
                return result != ValueStackItemType::Long;
            }

            /**
             * Adds two operands and returns the result. If one of the operands is of type real,
             * the computation will also be performed for that type.
             * @param x First operand.
             * @param y Second operand.
             * @return Returns the result of the operation.
             */
            static ValueStackItem add(ValueStackItem const& x, ValueStackItem const& y)
            {
                if (isreal(x, y))
                {
                    typedef real_type type;
                    return func<type>(x, y, [](type _x, type _y) -> type { return _x + _y; });
                }
                else
                {
                    typedef long_type type;
                    return func<type>(x, y, [](type _x, type _y) -> type { return _x + _y; });
                }
            }

            /**
             * Subtracts two operands and returns the result. If one of the operands is of type real,
             * the computation will also be performed for that type.
             * @param x First operand.
             * @param y Second operand.
             * @return Returns the result of the operation.
             */
            static ValueStackItem sub(ValueStackItem const& x, ValueStackItem const& y)
            {
                if (isreal(x, y))
                {
                    typedef real_type type;
                    return func<type>(x, y, [](type _x, type _y) -> type { return _x - _y; });
                }
                else
                {
                    typedef long_type type;
                    return func<type>(x, y, [](type _x, type _y) -> type { return _x - _y; });
                }
            }

            /**
             * Multiplies two operands and returns the result. If one of the operands is of type real,
             * the computation will also be performed for that type.
             * @param x First operand.
             * @param y Second operand.
             * @return Returns the result of the operation.
             */
            static ValueStackItem mul(ValueStackItem const& x, ValueStackItem const& y)
            {
                if (isreal(x, y))
                {
                    typedef real_type type;
                    return func<type>(x, y, [](type _x, type _y) -> type { return _x * _y; });
                }
                else
                {
                    typedef long_type type;
                    return func<type>(x, y, [](type _x, type _y) -> type { return _x * _y; });
                }
            }

            /**
             * Divides two operands and returns the result. The result is always of type real.
             * @param x First operand.
             * @param y Second operand.
             * @return Returns the result of the operation.
             */
            static ValueStackItem div(ValueStackItem const& x, ValueStackItem const& y)
            {
                typedef real_type type;
                return func<type>(x, y, [](type _x, type _y) -> type { return _x / _y; });
            }

            /**
             * Divides two operands and returns the result. The result is always of type long.
             * @param x First operand.
             * @param y Second operand.
             * @return Returns the result of the operation.
             */
            static ValueStackItem idiv(ValueStackItem const& x, ValueStackItem const& y)
            {
                typedef long_type type;
                return func<type>(x, y, [](type _x, type _y) -> type { return _x / _y; });
            }

            /**
             * Computes the remainder when dividing x by y. The result is always of type long.
             * @param x First operand.
             * @param y Second operand.
             * @return Returns the result of the operation.
             */
            static ValueStackItem mod(ValueStackItem const& x, ValueStackItem const& y)
            {
                typedef long_type type;
                return func<type>(x, y, [](type _x, type _y) -> type { return _x % _y; });
            }

            /**
             * Negates the provided value.
             * @param x First operand.
             * @return Returns the negated value.
             */
            static ValueStackItem neg(ValueStackItem const& x)
            {
                if (x.type().value() == ValueStackItemType::Real)
                {
                    return - x.toValueType<real_type>();
                }
                else
                {
                    return - x.toValueType<long_type>();
                }
            }

            /**
             * Performs the provided operation for the two values provided.
             * @param x First operand.
             * @param y Second operand.
             * @return Returns the operation's result.
             */
            template<typename ValueType, typename OperationType>
            inline static ValueType func(ValueStackItem const& x, ValueStackItem const& y, OperationType operation)
            {
                auto const lhs = x.toValueType<ValueType>();
                auto const rhs = y.toValueType<ValueType>();
                return operation(lhs, rhs);
            }
        };
    }

    /**************************************************************************
     * Inline operator implementation                                         *
     **************************************************************************/

    inline ValueStackItem operator+(ValueStackItem const& x, ValueStackItem const& y)
    {
        return detail::ValueStackItemOperation::add(x, y);
    }

    inline ValueStackItem operator-(ValueStackItem const& x, ValueStackItem const& y)
    {
        return detail::ValueStackItemOperation::sub(x, y);
    }

    inline ValueStackItem operator*(ValueStackItem const& x, ValueStackItem const& y)
    {
        return detail::ValueStackItemOperation::mul(x, y);
    }

    inline ValueStackItem operator/(ValueStackItem const& x, ValueStackItem const& y)
    {
        return detail::ValueStackItemOperation::div(x, y);
    }

    inline ValueStackItem operator%(ValueStackItem const& x, ValueStackItem const& y)
    {
        return detail::ValueStackItemOperation::mod(x, y);
    }

    inline ValueStackItem operator-(ValueStackItem const& x)
    {
        return detail::ValueStackItemOperation::neg(x);
    }
}
}

#endif  //__LIBFORMULA_UTIL_VALUESTACK_HPP
