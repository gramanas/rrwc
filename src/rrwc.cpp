#include <QDebug>

#include "rrwc.hpp"

Rrwc::Rrwc() {
    connect(&m_outputManager, SIGNAL(progressChanged(int)),
            this, SLOT(onProgressChanged(int)));
    connect(&m_outputManager, SIGNAL(done()),
            this, SLOT(onDone()));
}

void Rrwc::go(const QString &inputPath) {
    qDebug() << "Starting outputs...";
    for (int i = 0; i < m_outputManager.outputs().size(); i++) {
        m_outputManager.startOutput(i, inputPath);
    }
    qDebug() << "Done starting outputs";
    emit started();
}

void Rrwc::onProgressChanged(int progress) {
    emit progressChanged(progress);
}

void Rrwc::onDone() {
    qDebug() << "Cleaning output manager";
    m_outputManager.clean();
    qDebug() << "Job's done.";
    emit done(100);
}

Rrwc::~Rrwc() {
}
