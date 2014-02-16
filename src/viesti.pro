QT += core gui widgets
TARGET = viesti
TEMPLATE = app
CONFIG += c++11
CONFIG += debug

INCLUDEPATH += /usr/include/enchant
LIBS += -lenchant -lcld2 -lvmime


SOURCES += \
    compose/composewidget.cc \
    db/database.cc \
    db/mailmessage.cc \
    db/mailthread.cc \
    db/yearlydatabase.cc \
    main.cc \
    mainwindow.cc \
    qt5helper/enchanthighlighter.cc \
    settingsdialog.cc\
    util/digest.cc\

HEADERS += \
    compose/composewidget.hh \
    db/database.hh \
    db/globals.hh \
    db/mailmessage.hh \
    db/mailthread.hh \
    db/yearlydatabase.hh \
    mainwindow.hh \
    qt5helper/enchanthighlighter.hh \
    qt5helper/singletextedit.hh \
    settingsdialog.hh\
    util/digest.hh\
    util/fd.hh\

