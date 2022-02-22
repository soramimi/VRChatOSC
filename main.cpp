
#include "MainWindow.h"
#include <QApplication>

#ifdef _WIN32
#include <WinSock2.h>
void startup()
{
	WSAData wsaData;
	WSAStartup(MAKEWORD(2,0), &wsaData);
}
void cleanup()
{
	WSACleanup();
}
#else
void startup()
{
}
void cleanup()
{
}
#endif

int main(int argc, char *argv[])
{
	startup();

	QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

	QApplication a(argc, argv);
	MainWindow w;
	w.show();
	auto r = a.exec();

	cleanup();
	return r;
}
