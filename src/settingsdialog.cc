#include <QFileDialog>
#include <QFrame>
#include <QGridLayout>
#include <QHBoxLayout>
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

	gl->addWidget(new QLabel("From"), i, 0);
	from_ = new QTextEdit;
	from_->setPlainText(s_.value("mail_from").toStringList().join('\n'));
	gl->addWidget(from_, i++, 1);

	gl->addWidget(new QLabel("Sources"), i, 0);
	mail_sources_ = new QTextEdit;
	mail_sources_->setPlainText(s_.value("mail_sources").toStringList().join('\n'));
	gl->addWidget(mail_sources_, i++, 1);
  
	gl->addWidget(new QLabel("Database"), i, 0);
	mail_db_ = new QLineEdit;
	mail_db_->setText(s_.value("mail_db").toString());
	auto db_l = new QHBoxLayout;
	db_l->addWidget(mail_db_);
	auto db_b = new QPushButton("Select");
	connect(db_b, &QPushButton::pressed, [&]() {
			mail_db_->setText(QFileDialog::getExistingDirectory(this, "Database directory", "",  QFileDialog::ShowDirsOnly));
		});
	db_l->addWidget(db_b);
	gl->addLayout(db_l, i++, 1);


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
	s_.setValue("mail_from", from_->toPlainText().split('\n', QString::SkipEmptyParts));
	s_.setValue("mail_sources", mail_sources_->toPlainText().split('\n', QString::SkipEmptyParts));
	s_.setValue("mail_db", mail_db_->text());
	s_.setValue("mail_sort", mail_sort_->toPlainText());
	close();
}

SettingsDialog* SettingsDialog::instance() {
	static SettingsDialog s;
	return &s;
}
