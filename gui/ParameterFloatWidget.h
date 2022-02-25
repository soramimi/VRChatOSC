#ifndef PARAMETERFLOATWIDGET_H
#define PARAMETERFLOATWIDGET_H

#include "ParameterBoolWidget.h"

#include <QWidget>

namespace Ui {
class ParameterFloatWidget;
}

class ParameterFloatWidget : public AbstractParameterWidget {
	Q_OBJECT
private:
	Ui::ParameterFloatWidget *ui;
	QString address_;
public:
	explicit ParameterFloatWidget(QWidget *parent = nullptr);
	~ParameterFloatWidget();
	void setText(const QString &address, const QString &type);
private slots:
	void on_doubleSpinBox_valueChanged(double arg1);
};

#endif // PARAMETERFLOATWIDGET_H
