#include "ParameterBoolWidget.h"
#include "ui_ParameterBoolWidget.h"

#include <QDebug>

ParameterBoolWidget::ParameterBoolWidget(QWidget *parent)
	: AbstractParameterWidget(parent)
	, ui(new Ui::ParameterBoolWidget)
{
	ui->setupUi(this);
}

ParameterBoolWidget::~ParameterBoolWidget()
{
	delete ui;
}

void ParameterBoolWidget::setText(const QString &address, const QString &type)
{
	address_ = address;
	ui->label_address->setText(address);
	ui->label_type->setText(type);
}

void ParameterBoolWidget::on_checkBox_stateChanged(int arg1)
{
	bool checked = arg1 & Qt::Checked;
	QVariant v(checked);
	emit valueChanged(address_, v);
}
