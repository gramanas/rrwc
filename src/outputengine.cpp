#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <QDirIterator>
#include <QDebug>
#include <QObject>

#include "outputengine.hpp"
#include "globals.hpp"
#include "resizer.hpp"
#include "marker.hpp"
#include "renamer.hpp"
#include "exifmanager.hpp"

void OutputEngine::init(Output const * output,
                        QStringList &inputFiles,
                        const int current,
                        const int index) {
    m_index = index;
    m_inputFiles.swap(inputFiles);
    p_output = output;
    m_total = m_inputFiles.count();
    m_current = current;
}

void OutputEngine::run() {
    QStringListIterator it(m_inputFiles);
    cv::Mat source;
    cv::Mat watermark;
    cv::Mat out;
    QString filename;
    QString fullName;

    int doneByThisThread = 0;
    while(it.hasNext()) {
        QString path = it.next();
        QString type = path.split(".").back();
        filename = path.split(QDir::separator()).back().split(".").front();
        int progress = int((float(doneByThisThread) / float(m_total)) * 100);
        // qDebug() << "O:" << p_output->index << "\tT:" << m_index <<
        //     "\tC:" << m_current << "\tF:" << filename;
        QString logEntry = "O: " + QString::number(p_output->index) + "\tT: " + QString::number(m_index) +
            "\tC: " + QString::number(m_current) + "\tF: " + filename;
        emit writeLog(LOG_PROGRESS,
                      FILE_PROGRESS.arg(p_output->index).arg(
                        m_index).arg(filename + "." + type));
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

        // if strip exif data is on
        if (!p_output->stripMetadata) {
            ExifManager exifManager;
            exifManager.copyMetadata(path, fullName);
        }

        emit progressChanged(m_index, progress);
        m_current++;
        doneByThisThread++;
    }
    emit done();
}

