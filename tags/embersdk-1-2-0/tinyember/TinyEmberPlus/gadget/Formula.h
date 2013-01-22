#ifndef __TINYEMBER_GADGET_FORMULA_H
#define __TINYEMBER_GADGET_FORMULA_H

#include "../Types.h"

namespace gadget
{
    /** Forward delcarations */
    class Formula;

    bool operator==(Formula const& x, Formula const& y);
    bool operator!=(Formula const& x, Formula const& y);

    /**
     * Stores the two terms to compute the display value from the device value
     * and vice versa.
     */
    class Formula
    {
        friend bool operator==(Formula const& x, Formula const& y);
        friend bool operator!=(Formula const& x, Formula const& y);
        public:
            /**
             * Initializes a new Formula.
             * @param consumerToProvider The term to use when a display value needs to
             *      be transformed into a device value.
             * @param providerToConsumer The term to use when a device value needs to be
             *      transformed into a display value.
             */
            Formula(String const& consumerToProvider, String const& providerToConsumer);

            /**
             * Copy constructor.
             * @param other The Formula instance to copy the terms from.
             */
            Formula(Formula const& other);

            /**
             * Initializes an empty formula.
             */
            Formula();

            /**
             * Returns the term to use when computing a display value into a device value.
             * @teturn The term to use when computing a display value into a device value.
             */
            String const& consumerToProvider() const;

            /**
             * Returns the term to use when computing a device value into a display value.
             * @return The term to use when computing a device value into a display value.
             */
            String const& providerToConsumer() const;

            /**
             * Assignment operator, copies the passed formula.
             * @param other The formula to copy the terms from.
             * @return Returns the instance the new formula has been assigned to.
             */
            Formula &operator=(Formula const& other);

            /**
             * Returns true when no term is set.
             * @return true when no term is set.
             */
            bool empty() const;

            /**
             * Returns true when both terms are not empty.
             * @return true when both terms are not empty.
             */
            bool valid() const;

        private:
            String m_consumerToProvider;
            String m_providerToConsumer;
    };

    /**************************************************************************
     * Inline implementation                                                  *
     **************************************************************************/

    inline Formula::Formula(String const& providerToConsumer, String const& consumerToProvider)
        : m_consumerToProvider(consumerToProvider)
        , m_providerToConsumer(providerToConsumer)
    {
    }

    inline Formula::Formula(Formula const& other)
        : m_consumerToProvider(other.m_consumerToProvider)
        , m_providerToConsumer(other.m_providerToConsumer)
    {
    }

    inline Formula::Formula()
    {}

    inline bool Formula::empty() const
    {
        return m_consumerToProvider.length() == 0 && m_providerToConsumer.length() == 0;
    }

    inline bool Formula::valid() const
    {
        auto is_valid_term = [](String const& str) -> bool
        {
            auto const length = str.length();
            return length > 0
                ? str.at(0) == '(' && str.at(length - 1) == ')'
                : false;
        };

        return is_valid_term(m_consumerToProvider) 
            && is_valid_term(m_providerToConsumer);
    }

    inline String const& Formula::consumerToProvider() const
    {
        return m_consumerToProvider;
    }

    inline String const& Formula::providerToConsumer() const
    {
        return m_providerToConsumer;
    }

    inline Formula &Formula::operator=(Formula const& other)
    {
        m_consumerToProvider = other.m_consumerToProvider;
        m_providerToConsumer = other.m_providerToConsumer;
        return *this;
    }

    inline bool operator==(Formula const& x, Formula const& y)
    {
        return x.m_consumerToProvider == y.m_consumerToProvider && x.m_providerToConsumer == y.m_providerToConsumer;
    }

    inline bool operator!=(Formula const& x, Formula const& y)
    {
        return x.m_consumerToProvider != y.m_consumerToProvider || x.m_providerToConsumer != y.m_providerToConsumer;
    }
}

#endif//__TINYEMBER_GADGET_FORMULA_H
