#include "rrwc.hpp"

Rrwc::Rrwc(int argc, char *argv[])
    : m_app(argc,argv),
      m_window(0, &m_outputManager) {
}

int Rrwc::exec() {
    m_window.show();
    return m_app.exec();
}

Rrwc::~Rrwc() {
}
