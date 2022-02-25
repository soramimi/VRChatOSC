#ifndef VRCPARAMETERSCROLLAREA_H
#define VRCPARAMETERSCROLLAREA_H

#include <QScrollArea>
#include "../osclib/osc.h"

struct VRCParameter {
	struct IO {
		std::string address;
		osc::Value::Type type = osc::Value::Type::Void;
	};
	std::string name;
	IO input;
	IO output;
};

class VRCParameterScrollArea : public QScrollArea {
	Q_OBJECT
public:
	explicit VRCParameterScrollArea(QWidget *parent = nullptr);
	void makeContents(const std::vector<VRCParameter> &params);
	void clearContents();
signals:
	void parameterValueChanged(const QString &address, const QVariant &value);
};

#endif // VRCPARAMETERSCROLLAREA_H
