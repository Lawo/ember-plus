#ifndef __LIBEMBER_META_BOOLEAN_HPP
#define __LIBEMBER_META_BOOLEAN_HPP

namespace libember { namespace meta
{
    template<bool Constant>
    struct Boolean
    {
        typedef Boolean     type;
        static bool const   value = Constant;
        
        operator bool() const
        {
            return value;
        }
    };

    template<bool Constant>
    bool const Boolean<Constant>::value;

    typedef Boolean<true>   TrueType;
    typedef Boolean<false>  FalseType;
}
}

#endif // __LIBEMBER_META_BOOLEAN_HPP

