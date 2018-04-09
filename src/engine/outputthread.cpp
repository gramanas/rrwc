#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <QTime>
#include <QDebug>
#include <QObject>

#include "engine/outputthread.hpp"

OutputThread::OutputThread(const QVector<Output *> &outputs,
                           QVector<QString> &inputFiles,
                           Logger *logger,
                           const int current,
                           const int index)
    : m_outputs(outputs),
      m_current(current),
      p_logger(logger),
      m_index(index),
      m_engine(outputs, current, logger) {
    m_index = index;
    m_inputFiles.swap(inputFiles);
    m_total = m_inputFiles.count();
    m_current = current;
}

OutputThread::~OutputThread() {
}

void OutputThread::run() {
    auto it = m_inputFiles.begin();

    while(it != m_inputFiles.cend()) {
        const QString path = *it;
        p_logger->log(FILE_PROGRESS.arg(m_index + 1).arg(m_index + 1).arg(path.split(QDir::separator()).back()));

        m_engine.loadImage(path);

        for (const auto &it : m_outputs) {
            m_engine.setCurrentOutput(it->index);
            m_engine.exec();
            m_engine.write();
        }
        p_logger->incrementItemsDone();
        it++;
    }
    emit done();
}
