#include "rrwc.hpp"

Rrwc::Rrwc() {
}

Rrwc::~Rrwc() {
}

void Rrwc::go(QString inputPath) {
    EngineManager engineManager(&m_outputManager, inputPath);
    for (int i = 0; i < m_outputManager.outputs().size(); i++) {
        engineManager.startNew(i);
    }
}

