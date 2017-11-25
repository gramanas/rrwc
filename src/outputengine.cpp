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
                        const QDir &inputDir,
                        int index) {
    m_index = index;
    m_inputDir = inputDir;
    p_output = output;
    m_total = m_inputDir.count();
    m_filesPerThread = m_total;
}

void OutputEngine::run() {
    //    QDir dir(m_inputPath);
    //int total = m_inputDir.count();
    //int current = 0;
    QDirIterator it(m_inputDir);
    cv::Mat source;
    cv::Mat watermark;
    cv::Mat out;
    QString filename;
    QString fullName;

    int doneByThisThread = 0;
    while(it.hasNext() && doneByThisThread <= m_filesPerThread) {
        if (m_current < m_startingFilePosition) {
            it.next();
            m_current++;
            continue;
        }
        QString path = it.next();
        QString type = path.split(".").back();
        filename = path.split(QDir::separator()).back().split(".").front();
        qDebug() << "Thread" << m_index << "File" << filename;
        int progress = int((float(doneByThisThread) / float(m_filesPerThread)) * 100);

        // if ONLY rename is on
        if (p_output->rename && !p_output->resize && !p_output->watermark) {
            Renamer renamer(filename, p_output->renameText, p_output->counter, m_current);
            renamer.exec(filename);
            fullName = p_output->folder + QDir::separator() + filename + "." + type;
            QFile::copy(path, fullName);
            emit progressChanged(m_index, progress);
            m_current++;
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
            Renamer renamer(filename, p_output->renameText, p_output->counter, m_current);
            renamer.exec(filename);
        }

        // write
        fullName = p_output->folder + QDir::separator() + filename + "." + type;
        cv::imwrite(fullName.toStdString(), out);
        emit progressChanged(m_index, progress);
        m_current++;
        doneByThisThread++;
    }
    emit done();
}

        // cv::Rect roi(0,0,600,600);
        // cv::Mat out(source,roi);

