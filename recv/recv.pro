
TARGET = recv
TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

win32:LIBS += -lws2_32
unix:LIBS += -lpthread

SOURCES += \
	../misc/print.cpp \
	../osclib/osc.cpp \
	../osclib/sock.cpp \
	main.cpp

HEADERS += \
	../misc/print.h \
	../osclib/osc.h \
	../osclib/sock.h
