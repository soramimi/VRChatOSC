
#include "AbstractParameterWidget.h"
#include "ParameterBoolWidget.h"
#include "ParameterFloatWidget.h"
#include "ParameterIntWidget.h"
#include "VRCParameterScrollArea.h"

#include <QVBoxLayout>

VRCParameterScrollArea::VRCParameterScrollArea(QWidget *parent)
	: QScrollArea(parent)
{

}

void VRCParameterScrollArea::clearContents()
{
	QWidget *contents = widget();
	contents->deleteLater();
}

void VRCParameterScrollArea::makeContents(std::vector<VRCParameter> const &params)
{
	clearContents();

	QWidget *contents = new QWidget();
	setWidget(contents);

	QVBoxLayout *verticalLayout = new QVBoxLayout(contents);
	verticalLayout->setMargin(0);
	verticalLayout->setSpacing(0);

	for (VRCParameter const &param : params) {
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
			connect(paramwidget, &AbstractParameterWidget::valueChanged, this, &VRCParameterScrollArea::parameterValueChanged);
		}
	}

	verticalLayout->addItem(new QSpacerItem(20, 64, QSizePolicy::Minimum, QSizePolicy::Expanding));
}

