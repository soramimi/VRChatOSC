
TARGET = VRChatOSC
QT += core gui widgets
CONFIG += c++17

win32:LIBS += -lws2_32

SOURCES += \
    main.cpp \
    MainWindow.cpp \
    osc.cpp

HEADERS += \
    MainWindow.h \
    osc.h

FORMS += \
    MainWindow.ui

