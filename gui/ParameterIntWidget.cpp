#include "ParameterIntWidget.h"
#include "ui_ParameterIntWidget.h"

ParameterIntWidget::ParameterIntWidget(QWidget *parent)
	: AbstractParameterWidget(parent)
	, ui(new Ui::ParameterIntWidget)
{
	ui->setupUi(this);
}

ParameterIntWidget::~ParameterIntWidget()
{
	delete ui;
}

void ParameterIntWidget::setText(const QString &address, const QString &type)
{
	address_ = address;
	ui->label_address->setText(address);
	ui->label_type->setText(type);
}

void ParameterIntWidget::on_spinBox_valueChanged(int arg1)
{
	emit valueChanged(address_, QVariant(arg1));
}
