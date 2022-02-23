#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <chrono>
#include <thread>
#include "../misc/print.h"

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
	, ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	osc_listener_.received = [](char const *ptr, int len){
		print_hex_dump(ptr, len);
	};

	osc_listener_.value = [](std::string const &addr, osc::Value const &value){
		print_osc_value(addr, value);
	};

	osc_rx_.set_listener(&osc_listener_);

	char const *hostname = "127.0.0.1";
	osc_tx_.open(hostname);
	osc_rx_.open(hostname);
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
