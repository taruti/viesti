#include <QAction>
#include "compose/composedialog.hh"
#include "logwindow.hh"
#include "mainwindow.hh"
#include "settingsdialog.hh"

static ComposeDialog* composeDialog() {
	static ComposeDialog *sa = new ComposeDialog;
	return sa;
}

void fetchMessages();

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	setWindowTitle("viesti");
	// ctrl-f fetch messages
	auto act = new QAction(this);
	act->setShortcut(Qt::Key_F | Qt::CTRL);
	connect(act, &QAction::triggered, fetchMessages);
	this->addAction(act);
	// ctrl-l log window
	act = new QAction(this);
	act->setShortcut(Qt::Key_L | Qt::CTRL);
	connect(act, &QAction::triggered, []() { LogWindow::instance()->show(); });
	this->addAction(act);
	// ctrl-n compose message
	act = new QAction(this);
	act->setShortcut(Qt::Key_N | Qt::CTRL);
	connect(act, &QAction::triggered, []() { composeDialog()->show(); });
	this->addAction(act);
	// ctrl-p settings
	act = new QAction(this);
	act->setShortcut(Qt::Key_P | Qt::CTRL);
	connect(act, &QAction::triggered, []() { SettingsDialog::instance()->show(); });
	this->addAction(act);
	// ctrl-q quit
	act = new QAction(this);
	act->setShortcut(Qt::Key_Q | Qt::CTRL);
	connect(act, SIGNAL(triggered()), this, SLOT(close()));
	this->addAction(act);
}


