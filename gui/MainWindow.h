#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "../osclib/osc.h"
#include "VRCParameterScrollArea.h"
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
	Q_OBJECT
private:
	Ui::MainWindow *ui;
	struct Private;
	Private *m;
	static std::vector<VRCParameter> parseParametersJson(const QString path);
public:
	MainWindow(QWidget *parent = nullptr);
	~MainWindow();
	void setUserID(QString const &usr);
private slots:
	void onCtrlT();
	void onAvatarChanged(const QString &avtr);
	void parameterValueChanged(QString const &address, const QVariant &value);
signals:
	void avatarChanged(const QString &avtr);
};
#endif // MAINWINDOW_H
