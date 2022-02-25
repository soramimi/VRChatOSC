#include "SelectUserDialog.h"
#include "ui_SelectUserDialog.h"

SelectUserDialog::SelectUserDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::SelectUserDialog)
{
	ui->setupUi(this);
}

SelectUserDialog::~SelectUserDialog()
{
	delete ui;
}

void SelectUserDialog::addText(const QString &text)
{
	ui->listWidget->addItem(text);
}

QString SelectUserDialog::text()
{
	auto *item = ui->listWidget->currentItem();
	return item ? item->text() : QString();
}

void SelectUserDialog::on_listWidget_itemDoubleClicked(QListWidgetItem *item)
{
	done(QDialog::Accepted);
}
