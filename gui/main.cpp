
#include "MainWindow.h"
#include "SelectUserDialog.h"
#include <QApplication>
#include <QDirIterator>
#include <QMessageBox>
#include <QStandardPaths>
#include "../osclib/sock.h"

int main(int argc, char *argv[])
{
	sock::startup();

//	QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
//	QCoreApplication::setAttribute(Qt::AA_DisableHighDpiScaling);

	QApplication a(argc, argv);

	QString usr;
	{
		QStringList users;
		QString dir = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
		dir += "/AppData/LocalLow/VRChat/VRChat/OSC";
		QDirIterator it(dir);
		while (it.hasNext()) {
			it.next();
			QFileInfo info = it.fileInfo();
			if (info.isDir()) {
				QString name = info.fileName();
				if (name.startsWith("usr_")) {
					users.push_back(name);
				}
			}
		}

		if (users.empty()) {
			QMessageBox::warning(nullptr, QCoreApplication::tr("OSC not available"), "VRChatOSC");
		} else if (users.size() == 1) {
			usr = users[0];
		} else {
			SelectUserDialog dlg;
			for (int i = 0; i < users.size(); i++) {
				dlg.addText(users[i]);
			}
			if (dlg.exec() == QDialog::Accepted) {
				usr = dlg.text();
			}
		}
	}

	MainWindow w;
	w.setUserID(usr);
	w.show();
	auto r = a.exec();

	sock::cleanup();
	return r;
}
