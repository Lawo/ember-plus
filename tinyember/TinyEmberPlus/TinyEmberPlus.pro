# ----------------------------------------------------
# This file is generated by the Qt Visual Studio Add-in.
# ------------------------------------------------------

TEMPLATE = app
TARGET = TinyEmberPlus
DESTDIR = ./Win32/Release
QT += core gui network xml
CONFIG += release
DEFINES += LIBEMBER_HEADER_ONLY QT_LARGEFILE_SUPPORT QT_DLL QT_XML_LIB QT_NETWORK_LIB
INCLUDEPATH += ./../../libformula/Headers \
    ./../../libs101/Headers \
    ./../../libember/Headers \
    ./GeneratedFiles \
    . \
    ./GeneratedFiles/Release
PRECOMPILED_HEADER = StdAfx.h
DEPENDPATH += .
MOC_DIR += ./GeneratedFiles/release
OBJECTS_DIR += release
UI_DIR += ./GeneratedFiles
RCC_DIR += ./GeneratedFiles
include(TinyEmberPlus.pri)
win32:RC_FILE = TinyEmberPlus.rc