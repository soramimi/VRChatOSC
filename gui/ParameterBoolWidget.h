#ifndef PARAMETERBOOLWIDGET_H
#define PARAMETERBOOLWIDGET_H

#include "AbstractParameterWidget.h"

#include <QWidget>

namespace Ui {
class ParameterBoolWidget;
}

class ParameterBoolWidget : public AbstractParameterWidget {
	Q_OBJECT
private:
	Ui::ParameterBoolWidget *ui;
	QString address_;
public:
	explicit ParameterBoolWidget(QWidget *parent = nullptr);
	~ParameterBoolWidget();
	void setText(const QString &address, const QString &type) override;
private slots:
	void on_checkBox_stateChanged(int arg1);
};

#endif // PARAMETERBOOLWIDGET_H
