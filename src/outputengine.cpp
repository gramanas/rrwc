#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <QDirIterator>
#include <QDebug>
#include <QObject>
#include "outputengine.hpp"

void OutputEngine::init(Output const * output,
                        const QString &inputPath,
                        int index) {
    m_index = index;
    m_inputPath = inputPath;
    p_output = output;
}

void OutputEngine::run() {
    QDir dir(m_inputPath);
    dir.setFilter(QDir::Files);
    int total = dir.count();
    int current = 0;
    QDirIterator it(dir);
    cv::Mat im;

    while(it.hasNext()) {
        QString path = it.next();
        QString type = path.split(".").back();

        im = cv::imread(path.toStdString(), CV_LOAD_IMAGE_GRAYSCALE);
        cv::Rect roi(0,0,600,600);
        cv::Mat out(im,roi);
        QString filename = p_output->folder + "/" + path.split("/").back();
        cv::imwrite(filename.toStdString(), out);
        int progress = int((float(current) / float(total)) * 100);
        emit progressChanged(m_index, progress);
        current++;
    }
    emit done();
}
