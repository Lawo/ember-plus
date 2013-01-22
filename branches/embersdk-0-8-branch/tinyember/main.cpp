#include <QtGui/QApplication>
#include <QDataStream>
#include <QByteArray>
#include <QVariant>
#include <iostream>
#include <sstream>

#include "Libember.h"
#include "MainWindow.h"

#include "gadgets/EntityStateListener.h"
#include "gadgets/Node.h"
#include "gadgets/serialization/EntityArchive.h"

#include "glow/GlowClientMessageListener.h"
#include "glow/GlowStateVisitor.h"
#include "glow/GlowTcpInterface.h"

using namespace ::libember;
using namespace ::libember::glow;
using namespace ::gadgets;
using namespace ::glow;


/**
 * This class is the 'Link' between the incoming S101 message and the
 * user interface. It marshals decoded ember packets to the ui thread
 * which then handles the requests.
 */
class ClientMessageScheduler: public GlowClientMessageListener
{
public:
    /**
     * Constructor
     */
    ClientMessageScheduler()
        : m_syncTarget(0)
        , m_methodName("")
    {}

    /**
     * Sets the synchronization target that will be invoked when a new S101/Ember
     * message has been decoded.
     * @note Unfortunately the sync-target cannot be passed at construction, since it
     *      is used by two different classes.
     * @param syncTarget The object that will handle the s101 messages.
     * @param methodName The name of the method to call. The signature must have
     *      a void* parameter, which contains the decoded dom::Node.
     */
    void setSyncTarget(QObject* syncTarget, char const* methodName)
    {
        m_syncTarget = syncTarget;
        m_methodName = methodName;
    }

private:
    /**
     * @note See glow::GlowClientMessageListener
     */
    virtual void notify(dom::Node* node)
    {
        QObject *target = m_syncTarget;

        if (target != 0)
            QMetaObject::invokeMethod(target, m_methodName.toAscii(), Q_ARG(void*, node));
    }

private:
    QObject* m_syncTarget;
    QString m_methodName;
};


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    int result = -1;
    {
        ClientMessageScheduler scheduler;
        GlowTcpInterface interface(&app, &scheduler);
        MainWindow window(&interface, &interface);

        scheduler.setSyncTarget(&window, "synchronizedGlowRequest");
        window.show();

        result = app.exec();
    }
    return result;
}
