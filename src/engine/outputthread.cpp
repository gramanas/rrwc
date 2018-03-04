#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <QDirIterator>
#include <QTime>
#include <QDebug>
#include <QObject>

#include "engine/outputthread.hpp"

void OutputThread::init(Output const * output,
                        QStringList &inputFiles,
                        const int current,
                        const int index) {
    connect(&m_engine, SIGNAL(writeLog(QString, QString)),
            this, SLOT(onWriteLog(QString, QString)));
    m_index = index;
    m_inputFiles.swap(inputFiles);
    p_output = output;
    m_total = m_inputFiles.count();
    m_current = current;
    m_engine.init(output, current);
}

void OutputThread::run() {
    QStringListIterator it(m_inputFiles);

    int doneByThisThread = 0;
    int tmp;
    QTime a;
    a.start();
    while(it.hasNext()) {
        QString path = it.next();
        emit writeLog(LOG_PROGRESS,
                      FILE_PROGRESS.arg(p_output->index + 1).arg(
                        m_index + 1).arg(path.split(QDir::separator()).back()));

        m_engine.loadImage(path);

        m_engine.exec();

        m_engine.write();
        qDebug() << "Time till next" << a.elapsed();

        tmp = int((float(doneByThisThread) / float(m_total)) * 100);
        if (tmp != m_progress) {
            m_progress = tmp;
            emit progressChanged(m_index, m_progress);
        }
        doneByThisThread++;
        a.start();
    }
    emit done();
}

void OutputThread::onWriteLog(QString log, QString str) {
    emit writeLog(log, str);
}
