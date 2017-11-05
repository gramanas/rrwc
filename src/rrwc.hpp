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
    Rrwc(int argc, char *argv[]);
    ~Rrwc();

    int exec();

  private:
    QApplication m_app;
    MainWindow m_window;
    OutputManager m_outputManager;
};

#endif
