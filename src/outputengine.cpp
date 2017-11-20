#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <QDirIterator>
#include <QDebug>
#include <QObject>
#include "outputengine.hpp"
#include "resizer.hpp"
#include "marker.hpp"

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
    cv::Mat source;
    cv::Mat resized;
    cv::Mat watermark;
    cv::Mat out;

    while(it.hasNext()) {
        QString path = it.next();
        QString type = path.split(".").back();

        source = cv::imread(path.toStdString());
        if (p_output->watermark) {
            if (p_output->resize) {
                Resizer resizer(source, p_output->length, p_output->height);
                resizer.exec(resized);
                source = resized;
            }
            watermark = cv::imread(p_output->watermarkText.toStdString(), cv::IMREAD_UNCHANGED);
            Marker marker(source, watermark, p_output->opacity);
            marker.exec(out);
        }
        // cv::Rect roi(0,0,600,600);
        // cv::Mat out(source,roi);
        QString filename = p_output->folder + "/" + path.split("/").back();
        cv::imwrite(filename.toStdString(), out);
        int progress = int((float(current) / float(total)) * 100);
        emit progressChanged(m_index, progress);
        current++;
    }
    emit done();
}
