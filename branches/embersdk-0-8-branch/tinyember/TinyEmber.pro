#-------------------------------------------------
#
# Project created by QtCreator 2012-01-17T20:49:55
#
#-------------------------------------------------

QT       += core gui network

TARGET = TinyEmber
TEMPLATE = app

INCLUDEPATH += "../libember/Headers"
INCLUDEPATH += "../libs101/Headers"


SOURCES += main.cpp\
        MainWindow.cpp \
    gadgets/Entity.cpp \
    gadgets/Node.cpp \
    gadgets/Parameter.cpp \
    gadgets/EntityTypeVisitor.cpp \
    gadgets/states/StringState.cpp \
    gadgets/states/EnumState.cpp \
    gadgets/State.cpp \
    NodeView.cpp \
    TreeWidgetItemData.cpp \
    EntityViewFactory.cpp \
    IntParameterView.cpp \
    RealParameterView.cpp \
    StringParameterView.cpp \
    EnumParameterView.cpp \
    EditEnumerationDialog.cpp \
    gadgets/EntityStateVisitor.cpp \
    GadgetTreeViewContextMenu.cpp \
    CreateNodeDialog.cpp \
    CreateParameterDialog.cpp \
    net/TcpClientFactory.cpp \
    net/TcpClient.cpp \
    net/TcpServer.cpp \
    glow/GlowStateVisitor.cpp \
    glow/GlowElementVisitor.cpp \
    glow/GlowParameterVisitor.cpp \
    glow/GlowNodeVisitor.cpp \
    glow/GlowTcpClient.cpp \
    glow/GlowTcpInterface.cpp \
    glow/GlowClientMessageHandler.cpp

HEADERS  += MainWindow.h \
    gadgets/Entity.h \
    gadgets/DirtyState.h \
    gadgets/Node.h \
    functors/functors.h \
    gadgets/Parameter.h \
    gadgets/EntityTypeVisitor.h \
    gadgets/Property.h \
    gadgets/states/NumericState.h \
    gadgets/states/StringState.h \
    gadgets/states/EnumState.h \
    gadgets/serialization/EntityArchive.h \
    gadgets/State.h \
    gadgets/serialization/EntityWriter.h \
    gadgets/serialization/EntitySerialization.h \
    gadgets/serialization/EntityReader.h \
    NodeView.h \
    Types.h \
    TreeWidgetItemData.h \
    EntityViewFactory.h \
    IntParameterView.h \
    RealParameterView.h \
    StringParameterView.h \
    EnumParameterView.h \
    EditEnumerationDialog.h \
    gadgets/EntityStateVisitor.h \
    GadgetTreeViewContextMenu.h \
    CreateNodeDialog.h \
    CreateParameterDialog.h \
    gadgets/EntityStateListener.h \
    net/TcpClientFactory.h \
    net/TcpClient.h \
    net/TcpServer.h \
    glow/GlowStateVisitor.h \
    glow/GlowElementVisitor.h \
    glow/GlowParameterVisitor.h \
    glow/GlowNodeVisitor.h \
    Libember.h \
    glow/GlowTcpClient.h \
    glow/GlowClientMessageListener.h \
    glow/GlowTcpInterface.h \
    glow/GlowClientMessageHandler.h \
    glow/GlowStreamWriter.h

FORMS    += MainWindow.ui \
    NodeView.ui \
    IntParameterView.ui \
    RealParameterView.ui \
    StringParameterView.ui \
    EnumParameterView.ui \
    EditEnumerationDialog.ui \
    CreateNodeDialog.ui \
    CreateParameterDialog.ui

RESOURCES += \
    Resource.qrc

































































































































































