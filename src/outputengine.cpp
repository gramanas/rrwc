#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <QDirIterator>
#include <QDebug>
#include <QObject>
#include "outputengine.hpp"
#include "resizer.hpp"
#include "marker.hpp"
#include "renamer.hpp"

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
    cv::Mat watermark;
    cv::Mat out;
    QString filename;
    QString fullName;


    while(it.hasNext()) {
        QString path = it.next();
        QString type = path.split(".").back();
        filename = path.split("/").back().split(".").front();
        int progress = int((float(current) / float(total)) * 100);

        // if ONLY rename is on
        if (p_output->rename && !p_output->resize && !p_output->watermark) {
            Renamer renamer(filename, p_output->renameText, p_output->counter, current);
            renamer.exec(filename);
            fullName = p_output->folder + "/" + filename + "." + type;
            QFile::copy(path, fullName);
            emit progressChanged(m_index, progress);
            current++;
            continue;
        }

        // if watermark is on
        if (p_output->watermark) {
            source = cv::imread(path.toStdString());
            // AND resize
            if (p_output->resize) {
                Resizer resizer(source, p_output->length, p_output->height);
                resizer.exec(source);
            }
            // just watermark
            watermark = cv::imread(p_output->watermarkText.toStdString(), cv::IMREAD_UNCHANGED);
            Marker marker(source, watermark, p_output->opacity);
            marker.exec(out);
        }

        // if resize is on, but no watermark
        if (p_output->resize && !p_output->watermark) {
            source = cv::imread(path.toStdString());
            Resizer resizer(source, p_output->length, p_output->height);
            resizer.exec(out);
        }
        
        // if rename is on
        if (p_output->rename) {
            Renamer renamer(filename, p_output->renameText, p_output->counter, current);
            renamer.exec(filename);
        }

        // write
        fullName = p_output->folder + "/" + filename + "." + type;
        cv::imwrite(fullName.toStdString(), out);
        emit progressChanged(m_index, progress);
        current++;
    }
    emit done();
}

        // cv::Rect roi(0,0,600,600);
        // cv::Mat out(source,roi);

