#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <chrono>
#include <thread>

void dump(char const *ptr, int len)
{
	int offset = 0;
	while (offset < len) {
		char line[100];
		memset(line, ' ', sizeof(line));
		sprintf(line, "%04X ", offset);
		for (int i = 0; i < 16; i++) {
			if (offset + i < len) {
				int c = (unsigned char)ptr[offset + i];
				sprintf(line + 5 + 3 * i, "%02X ", c);
			} else {
				strcpy(line + 5 + 3 * i, "   ");
			}
		}
		for (int i = 0; i < 16; i++) {
			if (offset + i < len) {
				int c = (unsigned char)ptr[offset + i];
				if (c < 0x20 || c > 0x7f) {
					c = '.';
				}
				line[5 + 3 * 16 + i] = c;
			} else {
				line[5 + 3 * 16 + i] = ' ';
			}
		}
		line[5 + 3 * 16 + 16] = 0;
		offset += 16;

		fprintf(stderr, "%s\n", line);
	}
}

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
	, ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	osc_listener_.received = [](char const *ptr, int len){
		dump(ptr, len);
	};

	osc_listener_.value = [](std::string const &addr, osc::Value const &value){
		switch (value.type()) {
		case osc::Value::Type::Void:
			fprintf(stderr, "void\n");
			break;
		case osc::Value::Type::Bool:
			fprintf(stderr, "%s bool %d\n", addr.c_str(), (int)value.bool_value());
			break;
		case osc::Value::Type::Int:
			fprintf(stderr, "%s int %d\n", addr.c_str(), (int)value.int_value());
			break;
		case osc::Value::Type::Float:
			fprintf(stderr, "%s float %f\n", addr.c_str(), value.float_value());
			break;
		}
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
