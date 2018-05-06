#-------------------------------------------------
#
# Project created by QtCreator 2017-11-27T19:22:49
#
#-------------------------------------------------

QT       += core gui sql widgets

TARGET = Memorized
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    database.cpp \
    cardsdialog.cpp \
    utils.cpp \
    plaincardeditorwidget.cpp \
    editcarddialog.cpp \
    cardeditorwidget.cpp \
    datetimedisplaydelegate.cpp \
    reviewdialog.cpp \
    cardreviewwidget.cpp \
    plaincardreviewwidget.cpp \
    reviewutils.cpp \
    sundown/autolink.c \
    sundown/buffer.c \
    sundown/markdown.c \
    sundown/stack.c \
    sundown/html_smartypants.c \
    sundown/html.c \
    sundown/houdini_html_e.c \
    sundown/houdini_href_e.c \
    usermanagementdialog.cpp

HEADERS += \
        mainwindow.h \
    database.h \
    cardsdialog.h \
    utils.h \
    plaincardeditorwidget.h \
    cardeditorwidget.h \
    editcarddialog.h \
    datetimedisplaydelegate.h \
    reviewdialog.h \
    cardreviewwidget.h \
    plaincardreviewwidget.h \
    reviewutils.h \
    sundown/autolink.h \
    sundown/buffer.h \
    sundown/html_blocks.h \
    sundown/markdown.h \
    sundown/stack.h \
    sundown/html.h \
    sundown/houdini.h \
    usermanagementdialog.h

FORMS += \
        mainwindow.ui \
    cardsdialog.ui \
    plaincardeditorwidget.ui \
    editcarddialog.ui \
    reviewdialog.ui \
    plaincardreviewwidget.ui \
    usermanagementdialog.ui

RESOURCES += \
    resources.qrc
