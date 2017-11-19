#ifndef RRWC_HPP
#define RRWC_HPP

#include <QString>
#include <QApplication>

#include "outputmanager.hpp"
#include "enginemanager.hpp"

// TODO: Add cli support, initializing the same stuff
// but without the GUI compartments

class Rrwc : public QObject {
    Q_OBJECT
  public:
    Rrwc();
    ~Rrwc();

    OutputManager * outputManager() {
        return &m_outputManager;
    }

    void go(QString inputPath);

  private:
    OutputManager m_outputManager;
};

#endif // RRWC_HPP
