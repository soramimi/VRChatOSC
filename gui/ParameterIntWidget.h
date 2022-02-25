#ifndef PARAMETERINTWIDGET_H
#define PARAMETERINTWIDGET_H

#include "ParameterBoolWidget.h"

#include <QWidget>

namespace Ui {
class ParameterIntWidget;
}

class ParameterIntWidget : public AbstractParameterWidget {
	Q_OBJECT
private:
	Ui::ParameterIntWidget *ui;
	QString address_;
public:
	explicit ParameterIntWidget(QWidget *parent = nullptr);
	~ParameterIntWidget();
	void setText(const QString &address, const QString &type);
private slots:
	void on_spinBox_valueChanged(int arg1);
};

#endif // PARAMETERINTWIDGET_H
