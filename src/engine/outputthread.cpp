#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <QTime>
#include <QDebug>
#include <QObject>

#include "engine/outputthread.hpp"

OutputThread::OutputThread(const QVector<Output *> &outputs,
                           QVector<QString> &inputFiles,
                           const int current,
                           const int index)
    : m_outputs(outputs),
      m_current(current),
      m_index(index),
      m_engine(outputs, current) {
    connect(&m_engine, SIGNAL(writeLog(QString, QString)),
            this, SLOT(onWriteLog(QString, QString)));
    m_index = index;
    m_inputFiles.swap(inputFiles);
    m_total = m_inputFiles.count();
    m_current = current;
}

OutputThread::~OutputThread() {

}

void OutputThread::run() {
    auto it = m_inputFiles.begin();

    int doneByThisThread = 0;
    int tmp;
    QTime a;
    a.start();
    while(it != m_inputFiles.cend()) {
        const QString path = *it;
        emit writeLog(LOG_PROGRESS,
                      FILE_PROGRESS.arg(m_index + 1).arg(
                        m_index + 1).arg(path.split(QDir::separator()).back()));

        m_engine.loadImage(path);

        for (const auto &it : m_outputs) {
            m_engine.setCurrentOutput(it->index);
            m_engine.exec();
            m_engine.write();
        }
        qDebug() << "Time till next" << a.elapsed();

        tmp = int((float(doneByThisThread) / float(m_total)) * 100);
        if (tmp != m_progress) {
            m_progress = tmp;
            emit progressChanged(m_index, m_progress);
        }
        doneByThisThread++;
        a.start();
        it++;
    }
    emit done();
}

void OutputThread::onWriteLog(QString log, QString str) {
    emit writeLog(log, str);
}
