######################################################################
# Communi
######################################################################

TEMPLATE = lib
CONFIG += static
TARGET = base

greaterThan(QT_MAJOR_VERSION, 4) {
    QT += widgets
    DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x000000
}

DESTDIR = $$BUILD_ROOT/lib
DEPENDPATH += $$PWD
INCLUDEPATH += $$PWD

HEADERS += $$PWD/browserplugin.h
HEADERS += $$PWD/bufferview.h
HEADERS += $$PWD/documentplugin.h
HEADERS += $$PWD/inputplugin.h
HEADERS += $$PWD/listdelegate.h
HEADERS += $$PWD/listplugin.h
HEADERS += $$PWD/listview.h
HEADERS += $$PWD/splitview.h
HEADERS += $$PWD/textbrowser.h
HEADERS += $$PWD/textdocument.h
HEADERS += $$PWD/textinput.h
HEADERS += $$PWD/topiclabel.h
HEADERS += $$PWD/treedelegate.h
HEADERS += $$PWD/treeitem.h
HEADERS += $$PWD/treeplugin.h
HEADERS += $$PWD/treewidget.h
HEADERS += $$PWD/viewplugin.h
HEADERS += $$PWD/windowplugin.h

SOURCES += $$PWD/bufferview.cpp
SOURCES += $$PWD/listdelegate.cpp
SOURCES += $$PWD/listview.cpp
SOURCES += $$PWD/splitview.cpp
SOURCES += $$PWD/textbrowser.cpp
SOURCES += $$PWD/textdocument.cpp
SOURCES += $$PWD/textinput.cpp
SOURCES += $$PWD/topiclabel.cpp
SOURCES += $$PWD/treedelegate.cpp
SOURCES += $$PWD/treeitem.cpp
SOURCES += $$PWD/treewidget.cpp

include(../../config.pri)
include(../backend/backend.pri)
include(../util/util.pri)