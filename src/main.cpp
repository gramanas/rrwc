#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "person.hpp"
#include "mainwindow.hpp"
#include <QString>
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    Person per(20, QString("Person"));
    cv::Mat im = cv::imread("emacsLeet.png");
    //cv::namedWindow("woa");
    //cv::imshow("woa", im);
    //cv::waitKey(5000);
    return a.exec();
}
