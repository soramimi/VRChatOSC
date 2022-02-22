
TARGET = VRChatOSC
QT += core gui widgets
CONFIG += c++17

win32:LIBS += -lws2_32

SOURCES += \
	../osclib/sock.cpp \
	main.cpp \
	MainWindow.cpp \
	../osclib/osc.cpp

HEADERS += \
	../osclib/sock.h \
	MainWindow.h \
	../osclib/osc.h

FORMS += \
	MainWindow.ui

