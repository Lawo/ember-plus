#ifndef GLOWCLIENTMESSAGELISTENER_H
#define GLOWCLIENTMESSAGELISTENER_H

/** Forward declarations **/
namespace libember { namespace dom
{
    class Node;
}
}

namespace glow
{
    /**
     * Interface which is used by the Glow Tcp Interface to notify decoded ember messages.
     */
    class GlowClientMessageListener
    {
    public:
        /** Destructor **/
        virtual ~GlowClientMessageListener()
        {}

        /**
         * Called by a tcp client when it received a valid s101 message which contains
         * an ember packet.
         * @param node The decoded ember node.
         */
        virtual void notify(libember::dom::Node* node) = 0;
    };
}

#endif // GLOWCLIENTMESSAGELISTENER_H
