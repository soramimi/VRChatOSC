#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "../osclib/osc.h"
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

struct VRCParameter {
	struct IO {
		std::string address;
		osc::Value::Type type = osc::Value::Type::Void;
	};
	std::string name;
	IO input;
	IO output;
};

class MainWindow : public QMainWindow {
	Q_OBJECT
private:
	Ui::MainWindow *ui;
	osc::Transmitter osc_tx_;
	osc::Receiver osc_rx_;
	osc::Listener osc_listener_;
	QString current_avtr_;
	std::vector<VRCParameter> vrc_params_;
	void clearScrollContents();
	void makeParametersUI();
public:
	MainWindow(QWidget *parent = nullptr);
	~MainWindow();
private slots:
	void on_pushButton_jump_pressed();
	void on_pushButton_jump_released();
	void onCtrlT();
	void onAvatarChanged(const QString &avtr);
	void parameterValueChanged(QString const &address, const QVariant &value);
};
#endif // MAINWINDOW_H
