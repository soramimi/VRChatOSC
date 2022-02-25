#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "../misc/print.h"
#include "joinpath.h"
#include "jstream.h"
#include <QFileInfo>
#include <QShortcut>
#include <QStandardPaths>

struct MainWindow::Private {
	osc::Transmitter osc_tx;
	osc::Receiver osc_rx;
	osc::Listener osc_listener;
	QString user_id;
	QString current_avatar;
	std::vector<VRCParameter> input_params;
	std::vector<VRCParameter> avatar_params;
};

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
	, ui(new Ui::MainWindow)
	, m(new Private)
{
	ui->setupUi(this);

	connect(ui->scrollArea_inputs, &VRCParameterScrollArea::parameterValueChanged, this, &MainWindow::parameterValueChanged);
	connect(ui->scrollArea_avatar_parameters, &VRCParameterScrollArea::parameterValueChanged, this, &MainWindow::parameterValueChanged);

	{
		QString path = "C:/develop/VRChatOSC/gui/input.json";
		m->input_params = parseParametersJson(path);
		ui->scrollArea_inputs->makeContents(m->input_params);
	}

	connect(this, &MainWindow::avatarChanged, this, &MainWindow::onAvatarChanged);

	m->osc_listener.received = [](char const *ptr, int len){
		print_hex_dump(ptr, len);
	};

	m->osc_listener.value = [&](std::string const &addr, osc::Value const &value){
		print_osc_value(addr, value);
		if (addr == "/avatar/change") {
			emit avatarChanged(QString::fromStdString(value.string_value()));
		}
	};

	m->osc_rx.set_listener(&m->osc_listener);

	char const *hostname = "127.0.0.1";
	m->osc_tx.open(hostname);
	m->osc_rx.open(hostname);

	connect(new QShortcut(QKeySequence("Ctrl+T"), this), &QShortcut::activated, this, &MainWindow::onCtrlT);
}

MainWindow::~MainWindow()
{
	m->osc_tx.close();
	m->osc_rx.close();
	delete m;
	delete ui;
}

void MainWindow::setUserID(const QString &usr)
{
	m->user_id = usr;
}

void MainWindow::onAvatarChanged(QString const &avtr)
{
	m->current_avatar = avtr;
	QString path = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
	path += "/AppData/LocalLow/VRChat/VRChat/OSC" / m->user_id / "Avatars";
	path = path / m->current_avatar + ".json";
	m->avatar_params = parseParametersJson(path);
	ui->scrollArea_avatar_parameters->makeContents(m->avatar_params);
}

std::vector<VRCParameter> MainWindow::parseParametersJson(QString const path)
{
	std::vector<VRCParameter> params;
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
						params.push_back(param);
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
		}
	}
	return params;
}

void MainWindow::parameterValueChanged(const QString &address, QVariant const &value)
{
	switch (value.type()) {
	case QVariant::Bool:
		m->osc_tx.send_bool(address.toStdString(), value.toBool());
		break;
	case QVariant::Int:
		m->osc_tx.send_int(address.toStdString(), value.toInt());
		break;
	case QVariant::Double:
		m->osc_tx.send_float(address.toStdString(), value.toFloat());
		break;
	}
}

void MainWindow::onCtrlT()
{
}

