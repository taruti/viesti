#include <QFrame>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QTabWidget>
#include <QVBoxLayout>

#include "settingsdialog.hh"

SettingsDialog::SettingsDialog(QWidget *parent) :
	QDialog(parent) {
	auto vb = new QVBoxLayout;
	auto tb = new QTabWidget;
	vb->addWidget(tb);

	auto mailFrame = new QFrame;
	int i = 0;
	auto gl = new QGridLayout;
	s_.beginGroup("mail");
	gl->addWidget(new QLabel("From"), i, 0);
	from_ = new QTextEdit;
	from_->setPlainText(s_.value("from").toStringList().join('\n'));
	gl->addWidget(from_, i++, 1);
	s_.endGroup();
	mailFrame->setLayout(gl);
	tb->addTab(mailFrame, "Mail");
	
	auto apply = new QPushButton("Apply");
	connect(apply, SIGNAL(pressed()), this, SLOT(accept()));
	vb->addWidget(apply);
	setLayout(vb);
}

void SettingsDialog::accept() {
	s_.beginGroup("mail");
	s_.setValue("from", from_->toPlainText().split('\n', QString::SkipEmptyParts));
	s_.endGroup();
	close();
}
