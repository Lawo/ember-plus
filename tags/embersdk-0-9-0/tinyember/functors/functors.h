#ifndef FUNCTORS_H
#define FUNCTORS_H

namespace functors
{
    /**
     * Simple "functor" which performs a delete operation
     */
    template<typename ObjectType>
    struct delete_object
    {
        typedef ObjectType object_type;

        /**
         * Calls delete on the provided object
         * @param object Object to delete.
         */
        void operator()(object_type* object) const
        {
            delete object;
        }
    };
}

#endif // FUNCTORS_H
