#include <QAction>
#include "compose/composewidget.hh"
#include "mainwindow.hh"
#include "settingsdialog.hh"

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	setWindowTitle("viesti");
	// ctrl-p settings
	auto act = new QAction(this);
	act->setShortcut(Qt::Key_P | Qt::CTRL);
	connect(act, &QAction::triggered, []() {
		auto s = new SettingsDialog;
		s->show();
	});
	this->addAction(act);
	// ctrl-q quit
	act = new QAction(this);
	act->setShortcut(Qt::Key_Q | Qt::CTRL);
	connect(act, SIGNAL(triggered()), this, SLOT(close()));
	this->addAction(act);
	setCentralWidget(new ComposeWidget);
}


