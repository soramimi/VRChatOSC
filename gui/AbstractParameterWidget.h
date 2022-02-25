#ifndef ABSTRACTPARAMETERWIDGET_H
#define ABSTRACTPARAMETERWIDGET_H

#include <QWidget>

class AbstractParameterWidget : public QWidget {
	Q_OBJECT
public:
	explicit AbstractParameterWidget(QWidget *parent = nullptr);
	virtual void setText(QString const &address, QString const &type) = 0;
signals:
	void valueChanged(QString const &address, QVariant const &v);
};

#endif // ABSTRACTPARAMETERWIDGET_H
