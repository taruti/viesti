#include <QAction>
#include "compose/composewidget.hh"
#include "mainwindow.hh"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("viesti");

    // ctrl-q quit
    auto act = new QAction(this);
    act->setShortcut(Qt::Key_Q | Qt::CTRL);
    connect(act, SIGNAL(triggered()), this, SLOT(close()));
    this->addAction(act);

    setCentralWidget(new ComposeWidget);
}


