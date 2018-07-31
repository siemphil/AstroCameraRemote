#-------------------------------------------------
#
# Project created by QtCreator 2018-07-31T12:24:37
#
#-------------------------------------------------

QT       += widgets

TARGET = BatchProcess
TEMPLATE = lib

DLL_DEPS += AstroBase

DEFINES += BATCHPROCESS_LIBRARY

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        BatchProcess.cpp \
    BatchProcess_SelectFilesDialog.cpp

HEADERS += \
        BatchProcess.h \
        batchprocess_global.h \ 
    BatchProcess_SelectFilesDialog.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

FORMS += \
    BatchProcess_SelectFilesDialog.ui

include($${top_srcdir}/common_post.pri)
