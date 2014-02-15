#include <QApplication>
#include "mainwindow.hh"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setOrganizationName("Violetti");
    a.setOrganizationDomain("violetti.org");
    a.setApplicationName("viesti");
    MainWindow w;
    w.show();
    return a.exec();
}
