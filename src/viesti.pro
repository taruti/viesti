QT += core gui widgets
TARGET = viesti
TEMPLATE = app
CONFIG += c++11
CONFIG += debug

INCLUDEPATH += /usr/include/enchant
LIBS += -lenchant -lcld2 -lvmime -lxapian -llz4 -llua


SOURCES += \
    compose/composedialog.cc \
    db/database.cc \
    db/mailmessage.cc \
    db/mailthread.cc \
    db/singledatabase.cc \
    fetchmessages.cc \
    logwindow.cc \
    main.cc \
    mainwindow.cc \
    qt5helper/enchanthighlighter.cc \
    settingsdialog.cc\
    util/digest.cc\

HEADERS += \
    compose/composedialog.hh \
    db/database.hh \
    db/globals.hh \
    db/mailmessage.hh \
    db/mailthread.hh \
    db/singledatabase.hh \
    logwindow.hh \
    mainwindow.hh \
    qt5helper/enchanthighlighter.hh \
    qt5helper/singletextedit.hh \
    settingsdialog.hh\
    util/common.hh\
    util/digest.hh\
    util/fd.hh\

