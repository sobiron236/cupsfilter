# #####################################################################
# Automatically generated by qmake (2.01a) ?? 22. ??? 14:23:53 2009
# #####################################################################
QT += network
CONFIG += warn_on \
    qt
TEMPLATE = app
TARGET = safeFatPrinter
DEPENDPATH += . \
    src \
    ui
INCLUDEPATH += .

# Input
HEADERS += src/asktheuser.h \
    src/config.h \
    src/dcontroller.h \
    src/netagent.h \
    src/proc_thread.h \
    src/simpleitem.h \
    src/startdlg.h \
    src/tech_global.h \
    src/workreport.h \
    src/template_painter.h
FORMS += ui/asktheuser.ui \
    ui/lastStep.ui \
    ui/startdlg.ui
SOURCES += src/asktheuser.cpp \
    src/dcontroller.cpp \
    src/main.cpp \
    src/netagent.cpp \
    src/proc_thread.cpp \
    src/SimpleItem.cpp \
    src/startdlg.cpp \
    src/workreport.cpp \
    src/template_painter.cpp
RESOURCES += images.qrc
