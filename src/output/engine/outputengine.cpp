#include <QDir>
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
    // check cv::emtpy() it might not work correctly
    if (p_output->resize) {
        resizer.activate();
        resizer.loadData(p_output->length, p_output->height);
    }
    if (p_output->rename) {
        renamer.activate();
        renamer.loadData(p_output->renameText, p_output->counter);
    }
    if (p_output->watermark) {
        m_watermark = cv::imread(p_output->watermarkText.toStdString(), cv::IMREAD_UNCHANGED);
        if (m_watermark.empty()) {
            // LOG
        } else {
            marker.activate();
            marker.loadData(&m_watermark, p_output->opacity);
        }
    }
    // set copy mode
    // If neither watermark nor resize is turned on
    // we don't need to load the image in openCV libraries
    // since we'll just copy it.
    if (!p_output->resize && !p_output->watermark) {
        m_copyFlag = true;
    } else {
        m_copyFlag = false;
    }

}

bool OutputEngine::loadImage(const QString &path) {
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
    return true;
}

bool OutputEngine::exec() {
    m_source.copyTo(m_temp);
    if (renamer.isOn()) {
        renamer.loadNext(m_sourceInfo.baseName(), m_current);
        renamer.exec(m_newName);
    } else {
        m_newName = m_sourceInfo.baseName();
    }
    if (resizer.isOn()) {
        resizer.loadSource(&m_temp);
        if (!resizer.exec(m_out)) {
            // LOG
        }
        m_out.copyTo(m_temp);
    }

    if (marker.isOn()) {
        marker.loadSource(&m_temp);
        if (!marker.exec(m_out)) {
            emit writeLog(LOG_ERROR, ERR_OTP.arg(p_output->index + 1) + " " + ERR_IMAGE.arg(m_sourceInfo.baseName()) + ERR_WATERMARK_FIT);
        }
        m_out.copyTo(m_temp);
    }

    // strip metadata

    m_temp.copyTo(m_out);
    return true;
}

bool OutputEngine::write() {
    QString fullName = p_output->folder + QDir::separator() + m_newName + "." + m_sourceInfo.suffix();
    if (m_copyFlag) {
        QFile::copy(m_sourceInfo.absoluteFilePath(), fullName);
    } else {
        if (!cv::imwrite(fullName.toStdString(), m_out)) {
            emit writeLog(LOG_ERROR, ERR_FAIL.arg(p_output->index + 1).arg(m_sourceInfo.baseName()));
            return false;
        }
    }
    m_current++;
    return true;
}
