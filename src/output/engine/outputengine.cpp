#include <QDir>
#include <QTime>
#include <QFile>
#include "output/engine/outputengine.hpp"
#include <QDebug>

OutputEngine::OutputEngine() {
}
OutputEngine::~OutputEngine() {
}

void OutputEngine::init(Output const *output, int current) {
    p_output = output;
    m_current = current;

    if (p_output->resize) {
        m_resizer.activate();
        m_resizer.loadData(p_output->length, p_output->height);
    }
    if (p_output->rename) {
        m_renamer.activate();
        m_renamer.loadData(p_output->renameText, p_output->counter);
    }
    if (p_output->watermark) {
        m_watermark = cv::imread(p_output->watermarkText.toStdString(), cv::IMREAD_UNCHANGED);
        if (!m_watermark.empty()) {
            m_marker.activate();
            m_marker.loadData(&m_watermark, p_output->opacity);
        }
    }
    // set copy mode
    // If neither watermark nor resize is turned on
    // we don't need to load the image in openCV
    // since we'll just copy it.
    m_copyFlag = !p_output->resize && !p_output->watermark ? true : false;
}

bool OutputEngine::loadImage(const QString &path) {
    QTime a;
    a.start();
    m_sourceInfo = QFileInfo(path);
    if (!m_sourceInfo.exists()) {
        // LOG
        return false;
    }
    if (!m_copyFlag) {
        m_source = cv::imread(path.toStdString());
        if (m_source.empty()) {
            // LOG
            return false;
        }
    }
    qDebug() << "Time to read" << path.split('/').back() << a.elapsed();
    return true;
}

bool OutputEngine::exec() {
    m_source.copyTo(m_out);
    if (m_renamer.isOn()) {
        m_renamer.loadNext(m_sourceInfo.baseName(), m_current);
        m_renamer.exec(m_newName);
    }
    else {
        m_newName = m_sourceInfo.baseName();
    }
    if (m_resizer.isOn()) {
        QTime a;
        a.start();
        m_resizer.loadSource(&m_out);
        if (!m_resizer.exec(m_out)) {
            // LOG
        }
        qDebug() << "Time to resize" << m_newName << a.elapsed();
        //m_out.copyTo(m_temp);
    }

    if (m_marker.isOn()) {
        QTime a;
        a.start();
        m_marker.loadSource(&m_out);
        if (!m_marker.exec(m_out)) {
            emit writeLog(LOG_ERROR, ERR_OTP.arg(p_output->index + 1) + " " + ERR_IMAGE.arg(m_sourceInfo.baseName()) + ERR_WATERMARK_FIT);
        }
        qDebug() << "Time to watermark" << m_newName << a.elapsed();
        //m_out.copyTo(m_temp);
    }

    // strip metadata

    //m_temp.copyTo(m_out);
    return true;
}

bool OutputEngine::write() {
    QTime a;
    a.start();

    QString fullName = p_output->folder + QDir::separator() + m_newName + "." + m_sourceInfo.suffix();
    if (m_copyFlag) {
        QFile::copy(m_sourceInfo.absoluteFilePath(), fullName);
    }
    else {
        if (!cv::imwrite(fullName.toStdString(), m_out)) {
            emit writeLog(LOG_ERROR, ERR_FAIL.arg(p_output->index + 1).arg(m_sourceInfo.baseName()));
            return false;
        }
    }
    m_current++;
    qDebug() << "Time to write" << m_newName << a.elapsed();
    return true;
}
