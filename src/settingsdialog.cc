#include <QGridLayout>
#include <QLabel>
#include <QTextEdit>

#include "settingsdialog.hh"

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent) {
	int i = 0;
	auto gl = new QGridLayout;
	gl->addWidget(new QLabel("From"), i, 0);
	auto te = new QTextEdit;
	gl->addWidget(te, i++, 1);
//	gl->addWidget(
	setLayout(gl);
}
