#ifndef SETTINGSDIALOG_HH
#define SETTINGSDIALOG_HH 

#include <QDialog>
#include <QSettings>

class SettingsDialog : public QDialog {
    Q_OBJECT
	QSettings s_;
public:
    SettingsDialog(QWidget *parent = nullptr);
};

#endif /* SETTINGSDIALOG_HH */
