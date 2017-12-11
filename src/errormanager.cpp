#include "errormanager.hpp"
#include "globals.hpp"

#include "opencv2/opencv.hpp"
//#include "opencv2/imgproc/imgproc.hpp"

ErrorManager::ErrorManager(const OutputManager &outputManager)
    : m_outputManager(outputManager){
}

bool ErrorManager::checkInputErrors(const QString &inputPath) {
    bool flag = true;
    if (QDir(inputPath).entryList(
          QStringList({"*.jpg", "*.JPG"}),
          QDir::Files).isEmpty()) {
        emit writeLog(LOG_ERROR, ERR_NO_INPUT_FILES);
        flag = false;
    }
    Output *p;
    for (int i = 0; i < m_outputManager.outputs().size(); i++) {
         p = m_outputManager.outputs()[i];
         if (p->watermark) {
             if (!QFile::exists(p->watermarkText)) {
                 emit writeLog(LOG_ERROR, ERR_OTP.arg(i) + ERR_WATERMARK_TEXT.arg(p->watermarkText));
                 flag = false;
             }
         }
         if (p->resize && p->watermark) {
             if (QFile::exists(p->watermarkText)) {
                 int max;
                 if (p->height > p->length) {
                     max = p->height;
                 } else {
                     max = p->length;
                 }
                 cv::Mat watermark = cv::imread(p->watermarkText.toStdString());
                 if (watermark.cols < max || watermark.rows < max) {
                     emit writeLog(LOG_ERROR, ERR_OTP.arg(i) + ERR_SMALL_WATERMARK.arg(watermark.cols).arg(watermark.rows));
                     flag = false;
                 }
             }
         }
    }
    return flag;
}

ErrorManager::~ErrorManager() {
}
