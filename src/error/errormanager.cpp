#include "opencv2/opencv.hpp"

#include "error/errormanager.hpp"
#include "globals.hpp"

ErrorManager::ErrorManager(const OutputManager &outputManager)
    : m_outputManager(outputManager) {
}

bool ErrorManager::checkInputErrors(const QString &inputPath) {
    // check input
    QFileInfo inputInfo(inputPath);
    if (!inputInfo.isDir()
        || !inputInfo.exists()
        || !inputInfo.isReadable()) {
        emit writeLog(LOG_ERROR, ERR_WRONG_INPUT);
        m_flag = false;
    }
    // check the outputs
    Output *p;
    for (int i = 0; i < m_outputManager.outputs().size(); i++) {
        p = m_outputManager.outputs()[i];
        // check output folder
        QFileInfo fi(p->folder);
        if (!fi.isDir() || !fi.isWritable()) {
            emit writeLog(LOG_ERROR, ERR_OTP.arg(i+1) + ERR_WRONG_OUTPUT);
            m_flag = false;
        }
        // check watermark
        if (p->watermark) {
            if (!QFile::exists(p->watermarkText)) {
                emit writeLog(LOG_ERROR, ERR_OTP.arg(i+1) + ERR_WATERMARK_TEXT.arg(p->watermarkText));
                m_flag = false;
            }
        }
        // check if watermark fits with the resize settings
        if (p->resize && p->watermark) {
            if (QFile::exists(p->watermarkText)) {
                int max;
                p->height > p->length ? max = p->height : max = p->length;
                cv::Mat watermark = cv::imread(p->watermarkText.toStdString());
                if (watermark.cols < max || watermark.rows < max) {
                    emit writeLog(LOG_ERROR, ERR_OTP.arg(i+1) + ERR_SMALL_WATERMARK.arg(watermark.cols).arg(watermark.rows));
                    m_flag = false;
                }
            }
        }
    }
    return m_flag;
}

ErrorManager::~ErrorManager() {
}
