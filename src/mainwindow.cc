#include <QAction>
#include "compose/composedialog.hh"
#include "mainwindow.hh"
#include "settingsdialog.hh"

static ComposeDialog* composeDialog() {
	static ComposeDialog *sa = new ComposeDialog;
	return sa;
}

static SettingsDialog* settingsDialog() {
	static SettingsDialog *sa = new SettingsDialog;
	return sa;
}

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	setWindowTitle("viesti");
	// ctrl-n compose message
	auto act = new QAction(this);
	act->setShortcut(Qt::Key_N | Qt::CTRL);
	connect(act, &QAction::triggered, []() { composeDialog()->show(); });
	this->addAction(act);
	// ctrl-p settings
	act = new QAction(this);
	act->setShortcut(Qt::Key_P | Qt::CTRL);
	connect(act, &QAction::triggered, []() { settingsDialog()->show(); });
	this->addAction(act);
	// ctrl-q quit
	act = new QAction(this);
	act->setShortcut(Qt::Key_Q | Qt::CTRL);
	connect(act, SIGNAL(triggered()), this, SLOT(close()));
	this->addAction(act);
}


