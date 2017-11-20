#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "person.hpp"
#include "mainwindow.hpp"
#include "rrwc.hpp"
#include <QString>
#include <QApplication>

int main(int argc, char *argv[]) {
    Rrwc rrwc;
    QApplication a(argc, argv);
    MainWindow w(0, &rrwc);
    w.show();
    return a.exec();
}
