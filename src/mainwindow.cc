#include <xapian.h>
#include <QAction>
#include <QSettings>
#include "compose/composedialog.hh"
#include "logwindow.hh"
#include "mainwindow.hh"
#include "settingsdialog.hh"
#include "db/database.hh"

static ComposeDialog* composeDialog() {
	static ComposeDialog *sa = new ComposeDialog;
	return sa;
}

void fetchMessages();

template<typename This, typename Lambda>
static void add_action(This obj, const QKeySequence &shortcut, Lambda lambda) {
	auto act = new QAction(obj);
	act->setShortcut(shortcut);
	QObject::connect(act, &QAction::triggered, lambda);
	obj->addAction(act);
}

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	QSettings s;
	setWindowTitle("viesti");
	// ctrl-f fetch messages
	add_action(this, Qt::Key_F | Qt::CTRL, fetchMessages);
	// ctrl-l log window
	add_action(this, Qt::Key_L | Qt::CTRL, []() {
		LogWindow::instance()->show();
	});
	// ctrl-n compose message
	add_action(this, Qt::Key_N | Qt::CTRL, []() {
		composeDialog()->show();
	});
	// ctrl-p settings
	add_action(this, Qt::Key_P | Qt::CTRL, []() {
		SettingsDialog::instance()->show();
	});
	// ctrl-q quit
	add_action(this, Qt::Key_Q | Qt::CTRL, [&]() {
		emit close();
	});

	try {
		Database::instance()->scan_subdirs_for_databases(s.value("mail_db").toString().toStdString());
	} catch(std::exception &e) {
		log(std::string("Exception: ")+e.what());
	}
}


