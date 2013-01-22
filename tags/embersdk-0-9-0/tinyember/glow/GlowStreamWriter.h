#ifndef GLOWSTREAMWRITER_H
#define GLOWSTREAMWRITER_H

namespace libember { namespace glow
{
    /** Forward declarations **/
    class GlowElementCollection;
}
}

namespace glow
{
    /**
     * Stream interface which is used to encode and write data to
     * a client or a client-collection.
     */
    class GlowStreamWriter
    {
    public:
        /** Destructor **/
        virtual ~GlowStreamWriter()
        {}

        /**
         * Encodes a glow element collection and transmits it.
         * @param collection Collection to encode and transmit.
         */
        virtual void write(libember::glow::GlowElementCollection const* collection) = 0;
    };
}

#endif // GLOWSTREAMWRITER_H
