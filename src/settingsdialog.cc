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

	gl->addWidget(new QLabel("Sources"), i, 0);
	mail_sources_ = new QTextEdit;
	mail_sources_->setPlainText(s_.value("sources").toStringList().join('\n'));
	gl->addWidget(mail_sources_, i++, 1);

	s_.endGroup();
	mailFrame->setLayout(gl);
	tb->addTab(mailFrame, "Mail");

	mail_sort_ = new QTextEdit;
	mail_sort_->setPlainText(s_.value("mail_sort").toString());
	tb->addTab(mail_sort_, "Mail sorting");
	
	auto apply = new QPushButton("Apply");
	connect(apply, SIGNAL(pressed()), this, SLOT(accept()));
	vb->addWidget(apply);
	setLayout(vb);
}

void SettingsDialog::accept() {
	s_.beginGroup("mail");
	s_.setValue("from", from_->toPlainText().split('\n', QString::SkipEmptyParts));
	s_.setValue("sources", mail_sources_->toPlainText().split('\n', QString::SkipEmptyParts));
	s_.endGroup();
	s_.setValue("mail_sort", mail_sort_->toPlainText());
	close();
}
