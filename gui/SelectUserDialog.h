#ifndef SELECTUSERDIALOG_H
#define SELECTUSERDIALOG_H

#include <QDialog>

namespace Ui {
class SelectUserDialog;
}

class QListWidgetItem;

class SelectUserDialog : public QDialog {
	Q_OBJECT
private:
	Ui::SelectUserDialog *ui;
public:
	explicit SelectUserDialog(QWidget *parent = nullptr);
	~SelectUserDialog();
	void addText(QString const &text);
	QString text();
private slots:
	void on_listWidget_itemDoubleClicked(QListWidgetItem *item);
};

#endif // SELECTUSERDIALOG_H
