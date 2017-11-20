#include <QDebug>

#include "rrwc.hpp"

Rrwc::Rrwc() {
}

/////////////////////////////////////////////////////
// create new EngineManager                        //
// it will later be deleted when the done() signal //
// is emmited from all the threads                 //
/////////////////////////////////////////////////////
void Rrwc::go(QString inputPath) {
    p_engineManager = new EngineManager(&m_outputManager, inputPath);
    // connect the signals
    connect(p_engineManager, SIGNAL(progressChanged(int)),
            this, SLOT(onProgressChanged(int)));
    connect(p_engineManager, SIGNAL(done()),
            this, SLOT(onDone()));

    // start the threads
    for (int i = 0; i < m_outputManager.outputs().size(); i++) {
        p_engineManager->startNew(i);
    }
    emit started();
}

void Rrwc::onProgressChanged(int progress) {
    emit progressChanged(progress);
}

void Rrwc::onDone() {
    // delete EngineManager
    delete p_engineManager;
    p_engineManager = nullptr;
    qDebug() << "Job's done.";
    emit done(100);
}

Rrwc::~Rrwc() {
}
