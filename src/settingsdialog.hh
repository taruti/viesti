#ifndef SETTINGSDIALOG_HH
#define SETTINGSDIALOG_HH

#include <QDialog>
#include <QSettings>
#include <QTextEdit>

class SettingsDialog : public QDialog {
	Q_OBJECT
	QSettings s_;
	QTextEdit *from_;
public:
	SettingsDialog(QWidget *parent = nullptr);
public slots:
	virtual void accept() override;
};

#endif /* SETTINGSDIALOG_HH */
