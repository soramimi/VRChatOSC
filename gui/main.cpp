
#include "MainWindow.h"
#include <QApplication>
#include "../osclib/sock.h"

int main(int argc, char *argv[])
{
	sock::startup();

	QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

	QApplication a(argc, argv);
	MainWindow w;
	w.show();
	auto r = a.exec();

	sock::cleanup();
	return r;
}
