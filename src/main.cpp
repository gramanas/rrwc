#include <QString>
#include <QApplication>

#include "gui/mainwindow.hpp"
#include "rrwc.hpp"

int main(int argc, char *argv[]) {
    Rrwc rrwc;
    QApplication a(argc, argv);
    MainWindow w(0, &rrwc);
    w.show();
    return a.exec();
}
