#ifndef __TINYEMBER_GADGET_FORMULA_H
#define __TINYEMBER_GADGET_FORMULA_H

#include "../Types.h"

namespace gadget
{
    class Formula;

    bool operator==(Formula const& x, Formula const& y);

    bool operator!=(Formula const& x, Formula const& y);

    class Formula
    {
        friend bool operator==(Formula const& x, Formula const& y);
        friend bool operator!=(Formula const& x, Formula const& y);
        public:
            Formula(String const& consumerToProvider, String const& providerToConsumer);

            Formula(Formula const& other);

            Formula();

            String const& consumerToProvider() const;

            String const& providerToConsumer() const;

            Formula &operator=(Formula const& other);

            bool empty() const;

            bool valid() const;

        private:
            String m_consumerToProvider;
            String m_providerToConsumer;
    };

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

        return is_valid_term( m_consumerToProvider ) 
            && is_valid_term( m_providerToConsumer );
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
