
TARGET = jump
TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

win32:LIBS += -lws2_32

SOURCES += \
	../osclib/osc.cpp \
	../osclib/sock.cpp \
	main.cpp

HEADERS += \
	../osclib/osc.h \
	../osclib/sock.h
