
TARGET = VRChatOSC
QT += core gui widgets
CONFIG += c++17

win32:LIBS += -lws2_32

SOURCES += \
	../misc/print.cpp \
	../osclib/sock.cpp \
	AbstractParameterWidget.cpp \
	ParameterBoolWidget.cpp \
	ParameterFloatWidget.cpp \
	ParameterIntWidget.cpp \
	SelectUserDialog.cpp \
	VRCParameterScrollArea.cpp \
	joinpath.cpp \
	main.cpp \
	MainWindow.cpp \
	../osclib/osc.cpp

HEADERS += \
	../misc/print.h \
	../osclib/sock.h \
	AbstractParameterWidget.h \
	MainWindow.h \
	../osclib/osc.h \
	ParameterBoolWidget.h \
	ParameterFloatWidget.h \
	ParameterIntWidget.h \
	SelectUserDialog.h \
	VRCParameterScrollArea.h \
	joinpath.h \
	jstream.h

FORMS += \
	MainWindow.ui \
	ParameterBoolWidget.ui \
	ParameterFloatWidget.ui \
	ParameterIntWidget.ui \
	SelectUserDialog.ui

DISTFILES += \
	input.json

