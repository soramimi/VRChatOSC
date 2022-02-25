#include "ParameterFloatWidget.h"
#include "ui_ParameterFloatWidget.h"

ParameterFloatWidget::ParameterFloatWidget(QWidget *parent)
	: AbstractParameterWidget(parent)
	, ui(new Ui::ParameterFloatWidget)
{
	ui->setupUi(this);
}

ParameterFloatWidget::~ParameterFloatWidget()
{
	delete ui;
}

void ParameterFloatWidget::setText(const QString &address, const QString &type)
{
	address_ = address;
	ui->label_address->setText(address);
	ui->label_type->setText(type);
}

void ParameterFloatWidget::on_doubleSpinBox_valueChanged(double arg1)
{
	emit valueChanged(address_, QVariant(arg1));
}
