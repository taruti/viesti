#ifndef SETTINGSDIALOG_HH
#define SETTINGSDIALOG_HH

#include <QDialog>
#include <QLineEdit>
#include <QSettings>
#include <QTextEdit>

class SettingsDialog : public QDialog {
	Q_OBJECT
	QSettings s_;
	QTextEdit *from_;
	QTextEdit *mail_sort_;
	QTextEdit *mail_sources_;
	QLineEdit *mail_db_;
public:
	SettingsDialog(QWidget *parent = nullptr);
	static SettingsDialog* instance();
public slots:
	virtual void accept() override;
};

#endif /* SETTINGSDIALOG_HH */
