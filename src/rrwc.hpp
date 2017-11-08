#ifndef RRWC_HPP
#define RRWC_HPP

#include <QString>
#include <QApplication>

#include "mainwindow.hpp"
#include "outputmanager.hpp"

// TODO: Add cli support, initializing the same stuff
// but without the GUI compartments

class Rrwc {
  public:
    Rrwc();
    ~Rrwc();

    OutputManager * outputManager() {
        return &m_outputManager;
    }

  private:
    OutputManager m_outputManager;
};

#endif