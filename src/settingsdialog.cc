#include <QGridLayout>
#include <QLabel>
#include <QPushButton>

#include "settingsdialog.hh"

SettingsDialog::SettingsDialog(QWidget *parent) :
	QDialog(parent) {
	int i = 0;
	auto gl = new QGridLayout;
	s_.beginGroup("mail");
	gl->addWidget(new QLabel("From"), i, 0);
	from_ = new QTextEdit;
	from_->setPlainText(s_.value("from").toStringList().join('\n'));
	gl->addWidget(from_, i++, 1);
	s_.endGroup();
	auto apply = new QPushButton("Apply");
	connect(apply, SIGNAL(pressed()), this, SLOT(accept()));
	gl->addWidget(apply, i, 0);
	setLayout(gl);
}

void SettingsDialog::accept() {
	s_.beginGroup("mail");
	s_.setValue("from", from_->toPlainText().split('\n', QString::SkipEmptyParts));
	s_.endGroup();
	close();
}
