#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "../osclib/osc.h"
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
	Q_OBJECT
private:
	Ui::MainWindow *ui;
	osc::Transmitter osc_tx_;
	osc::Receiver osc_rx_;
	osc::Listener osc_listener_;
public:
	MainWindow(QWidget *parent = nullptr);
	~MainWindow();
private slots:
	void on_pushButton_jump_pressed();
	void on_pushButton_jump_released();
};
#endif // MAINWINDOW_H
