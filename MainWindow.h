#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "osc.h"

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
	Q_OBJECT
private:
	osc::Transmitter osc_tx_;
	osc::Receiver osc_rx_;
	osc::Listener osc_listener_;
public:
	MainWindow(QWidget *parent = nullptr);
	~MainWindow();
private slots:
	void on_pushButton_jump_clicked();
private:
	Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
