#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <QDirIterator>
#include <QDebug>
#include <QObject>
#include "outputengine.hpp"

OutputEngine::OutputEngine(Output const *const output,
                           const QString &inputPath,
                           int index)
    : p_output(output),
      m_inputPath(inputPath),
      m_index(index) {
}

void OutputEngine::run() {
    QDir dir(m_inputPath);
    dir.setFilter(QDir::Files);
    int total = dir.count();
    QDirIterator it(dir);

    while(it.hasNext()) {
        QString path = it.next();
        QString type = path.split(".").back();

        cv::Mat im = cv::imread(path.toStdString(), CV_LOAD_IMAGE_GRAYSCALE);
        cv::Rect roi(0,0,600,600);
        cv::Mat out(im,roi);
        QString filename = p_output->folder + "/" + path.split("/").back();
        qDebug() << "Thread: " << m_index << "File: " << filename;
        cv::imwrite(filename.toStdString(), out);
        //        float progress = (float(current) / float(total)) * 100;
    }
}


OutputEngine::~OutputEngine() {
}
