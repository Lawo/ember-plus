# ----------------------------------------------------
# This file is generated by the Qt Visual Studio Add-in.
# ------------------------------------------------------

# This is a reminder that you are using a generated .pro file.
# Remove it when you are finished editing this file.
message("You are running qmake on a generated .pro file. This may not work!")


HEADERS += ./GadgetViewContextMenu.h \
    ./TreeWidgetItemData.h \
    ./Types.h \
    ./ViewFactory.h \
    ./gadget/Access.h \
    ./gadget/Collection.h \
    ./gadget/DirtyState.h \
    ./gadget/DirtyStateListener.h \
    ./gadget/EnumParameter.h \
    ./gadget/Formula.h \
    ./gadget/IntegerParameter.h \
    ./gadget/Node.h \
    ./gadget/NodeFactory.h \
    ./gadget/NodeField.h \
    ./gadget/Parameter.h \
    ./gadget/ParameterFactory.h \
    ./gadget/ParameterField.h \
    ./gadget/ParameterType.h \
    ./gadget/ParameterTypeVisitor.h \
    ./gadget/RealParameter.h \
    ./gadget/StreamDescriptor.h \
    ./gadget/StreamFormat.h \
    ./gadget/StreamManager.h \
    ./gadget/StringParameter.h \
    ./gadget/Subscriber.h \
    ./gadget/util/EntityPath.h \
    ./gadget/util/NumberFactory.h \
    ./glow/Consumer.h \
    ./glow/ConsumerProxy.h \
    ./glow/ConsumerRequestProcessor.h \
    ./glow/Encoder.h \
    ./glow/ProviderInterface.h \
    ./glow/Settings.h \
    ./glow/util/NodeConverter.h \
    ./glow/util/ParameterConverter.h \
    ./glow/util/StreamConverter.h \
    ./net/TcpClientFactory.h \
    ./net/TcpServer.h \
    ./net/TcpClient.h \
    ./serialization/Archive.h \
    ./serialization/SettingsSerializer.h \
    ./serialization/detail/GadgetTreeReader.h \
    ./serialization/detail/GadgetTreeWriter.h \
    ./util/StringConverter.h \
    ./util/StreamFormatConverter.h \
    ./TinyEmberPlus.h \
    ./StringView.h \
    ./RealView.h \
    ./NodeView.h \
    ./IntegerView.h \
    ./EnumerationView.h \
    ./EditEnumerationDialog.h \
    ./CreateParameterDialog.h \
    ./CreateNodeDialog.h
SOURCES += ./GadgetViewContextMenu.cpp \
    ./main.cpp \
    ./ViewFactory.cpp \
    ./gadget/EnumParameter.cpp \
    ./gadget/IntegerParameter.cpp \
    ./gadget/Node.cpp \
    ./gadget/NodeFactory.cpp \
    ./gadget/Parameter.cpp \
    ./gadget/ParameterFactory.cpp \
    ./gadget/RealParameter.cpp \
    ./gadget/StreamManager.cpp \
    ./gadget/StringParameter.cpp \
    ./gadget/Subscriber.cpp \
    ./gadget/util/EntityPath.cpp \
    ./glow/Consumer.cpp \
    ./glow/ConsumerProxy.cpp \
    ./glow/ConsumerRequestProcessor.cpp \
    ./glow/Encoder.cpp \
    ./glow/util/NodeConverter.cpp \
    ./glow/util/ParameterConverter.cpp \
    ./glow/util/StreamConverter.cpp \
    ./net/TcpClient.cpp \
    ./net/TcpServer.cpp \
    ./serialization/Archive.cpp \
    ./serialization/SettingsSerializer.cpp \
    ./serialization/detail/GadgetTreeReader.cpp \
    ./serialization/detail/GadgetTreeWriter.cpp \
    ./util/StreamFormatConverter.cpp \
    ./CreateNodeDialog.cpp \
    ./CreateParameterDialog.cpp \
    ./EditEnumerationDialog.cpp \
    ./EnumerationView.cpp \
    ./IntegerView.cpp \
    ./NodeView.cpp \
    ./RealView.cpp \
    ./StringView.cpp \
    ./TinyEmberPlus.cpp
FORMS += ./TinyEmberPlus.ui \
    ./CreateNodeDialog.ui \
    ./CreateParameterDialog.ui \
    ./EditEnumerationDialog.ui \
    ./EnumerationView.ui \
    ./IntegerView.ui \
    ./NodeView.ui \
    ./RealView.ui \
    ./StringView.ui
RESOURCES += TinyEmberPlus.qrc
