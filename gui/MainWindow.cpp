#include "MainWindow.h"
#include "ParameterFloatWidget.h"
#include "ParameterIntWidget.h"
#include "ui_MainWindow.h"
#include <QDebug>
#include <QFileInfo>
#include <QShortcut>
#include <QStandardPaths>
#include <chrono>
#include <thread>
#include "../misc/print.h"
#include "joinpath.h"
#include "jstream.h"

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
	, ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	onAvatarChanged("avtr_c6313228-ae12-4663-abef-2a8352c4707c");

	osc_listener_.received = [](char const *ptr, int len){
//		print_hex_dump(ptr, len);
	};

	osc_listener_.value = [&](std::string const &addr, osc::Value const &value){
		print_osc_value(addr, value);
		if (addr == "/avatar/change") {
			onAvatarChanged(QString::fromStdString(value.string_value()));
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

void MainWindow::clearScrollContents()
{
	while (!ui->scrollAreaWidgetContents->children().empty()) {
		QObject *o = ui->scrollAreaWidgetContents->children().last();
		if (o) {
			AbstractParameterWidget *w = qobject_cast<AbstractParameterWidget *>(o);
			if (w) {
				disconnect(w, &AbstractParameterWidget::valueChanged, this, &MainWindow::parameterValueChanged);
			}
			delete o;
		}
	}
}

void MainWindow::makeParametersUI()
{
	clearScrollContents();

	QVBoxLayout *verticalLayout = new QVBoxLayout(ui->scrollAreaWidgetContents);

	for (VRCParameter const &param : vrc_params_) {
		AbstractParameterWidget *paramwidget = nullptr;
		QString type;
		switch (param.input.type) {
		case osc::Value::Type::Bool:
			paramwidget = new ParameterBoolWidget();
			type = "bool";
			break;
		case osc::Value::Type::Int:
			paramwidget = new ParameterIntWidget();
			type = "int";
			break;
		case osc::Value::Type::Float:
			paramwidget = new ParameterFloatWidget();
			type = "float";
			break;
		}
		if (paramwidget) {
			paramwidget->setText(QString::fromStdString(param.input.address), type);
			verticalLayout->addWidget(paramwidget);
			connect(paramwidget, &AbstractParameterWidget::valueChanged, this, &MainWindow::parameterValueChanged);
		}
	}



	verticalLayout->addItem(new QSpacerItem(20, 64, QSizePolicy::Minimum, QSizePolicy::Expanding));
}

void MainWindow::on_pushButton_jump_pressed()
{
	osc_tx_.send_int("/input/Jump", 1);

}

void MainWindow::on_pushButton_jump_released()
{
	osc_tx_.send_int("/input/Jump", 0);

}

void MainWindow::onAvatarChanged(QString const &avtr)
{
	current_avtr_ = avtr;
	makeParametersUI();
}

void MainWindow::onCtrlT()
{
	vrc_params_.clear();
	QString path = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
	path += "/AppData/LocalLow/VRChat/VRChat/OSC/usr_3a807194-8f0e-4bb0-bfbb-b8806b30f5a5/Avatars";
	path = path / current_avtr_ + ".json";
	path = "C:/develop/VRChatOSC/gui/input.json";
	QFileInfo info(path);
	if (info.isFile()) {
		QFile file(path);
		file.open(QFile::ReadOnly);
		QByteArray ba = file.readAll();
		if (!ba.isEmpty()) {
			char const *begin = ba.data();
			char const *end = begin + ba.size();
			if (begin + 3 < end) {
				if (memcmp(begin, "\xef\xbb\xbf", 3) == 0) {
					begin += 3;
				}
			}
			std::string avtrname;
			VRCParameter param;
			jstream::Reader r(begin, end);
			auto ValueType = [](std::string const &t){
				if (stricmp(t.c_str(), "bool") == 0) {
					return osc::Value::Type::Bool;
				} else if (stricmp(t.c_str(), "Int") == 0) {
					return osc::Value::Type::Int;
				} else if (stricmp(t.c_str(), "Float") == 0) {
					return osc::Value::Type::Float;
				}
				return osc::Value::Type::Void;
			};
			while (r.next()) {
				if (r.match("{name")) {
					avtrname = r.string();
				} else if (r.match("{parameters[{")) {
					if (r.state() == jstream::StartObject) {
						param = {};
					}
				} else if (r.match("{parameters[")) {
					if (r.state() == jstream::EndObject) {
						vrc_params_.push_back(param);
						param = {};
					}
				} else if (r.match("{parameters[{name")) {
					param.name = r.string();
				} else if (r.match("{parameters[{input{address")) {
					param.input.address = r.string();
				} else if (r.match("{parameters[{input{type")) {
					param.input.type = ValueType(r.string());
				} else if (r.match("{parameters[{output{address")) {
					param.output.address = r.string();
				} else if (r.match("{parameters[{output{type")) {
					param.output.type = ValueType(r.string());
				}
			}
			makeParametersUI();
		}

	}

}

void MainWindow::parameterValueChanged(const QString &address, QVariant const &value)
{
	switch (value.type()) {
	case QVariant::Bool:
		osc_tx_.send_bool(address.toStdString(), value.toBool());
		break;
	case QVariant::Int:
		osc_tx_.send_int(address.toStdString(), value.toInt());
		break;
	case QVariant::Double:
		osc_tx_.send_float(address.toStdString(), value.toFloat());
		break;
	}
}

