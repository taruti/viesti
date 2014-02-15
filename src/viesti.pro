QT += core gui widgets
TARGET = viesti
TEMPLATE = app
CONFIG += c++11
CONFIG += debug

INCLUDEPATH += /usr/include/enchant
LIBS += -lenchant -lcld2


SOURCES += \
    compose/composewidget.cc \
    main.cc \
    mainwindow.cc \
    qt5helper/enchanthighlighter.cc \
    settingsdialog.cc\

HEADERS += \
    compose/composewidget.hh \
    mainwindow.hh \
    qt5helper/enchanthighlighter.hh \
    qt5helper/singletextedit.hh \
    settingsdialog.hh\

