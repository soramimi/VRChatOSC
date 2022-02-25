#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QDebug>
#include <QFileInfo>
#include <QShortcut>
#include <QStandardPaths>
#include <chrono>
#include <thread>
#include "../misc/print.h"

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
	, ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	current_avtr_ = "avtr_c6313228-ae12-4663-abef-2a8352c4707c";

	osc_listener_.received = [](char const *ptr, int len){
//		print_hex_dump(ptr, len);
	};

	osc_listener_.value = [&](std::string const &addr, osc::Value const &value){
		print_osc_value(addr, value);
		if (addr == "/avatar/change") {
			current_avtr_ = QString::fromStdString(value.string_value());
		}
	};

	osc_rx_.set_listener(&osc_listener_);

	char const *hostname = "127.0.0.1";
	osc_tx_.open(hostname);
	osc_rx_.open(hostname);

	connect(new QShortcut(QKeySequence("Ctrl+T"), this), &QShortcut::activated, this, &MainWindow::onCtrlT);
}

MainWindow::~MainWindow()
{
	osc_tx_.close();
	osc_rx_.close();
	delete ui;
}

void MainWindow::on_pushButton_jump_pressed()
{
	osc_tx_.send_int("/input/Jump", 1);

}

void MainWindow::on_pushButton_jump_released()
{
	osc_tx_.send_int("/input/Jump", 0);

}

void MainWindow::onCtrlT()
{
	QString path = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
	path += "/AppData/LocalLow";
	path += "/VRChat/VRChat/OSC/usr_3a807194-8f0e-4bb0-bfbb-b8806b30f5a5/Avatars/";
	path += current_avtr_ + ".json";
	QFileInfo info(path);
	qDebug() << path << info.isFile();

}
