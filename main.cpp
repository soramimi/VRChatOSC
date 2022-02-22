
#include "MainWindow.h"
#include <QApplication>
#include <WinSock2.h>


int main(int argc, char *argv[])
{
	WSAData wsaData;
	WSAStartup(MAKEWORD(2,0), &wsaData);

	QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

	QApplication a(argc, argv);
	MainWindow w;
	w.show();
	auto r = a.exec();

	WSACleanup();
	return r;
}
