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

  public slots:
    void onProgressChanged(int progress);
    void onDone();

  signals:
    void progressChanged(int progress);
    void started();
    void done(int);

  private:
    OutputManager m_outputManager;
    EngineManager *p_engineManager;
};

#endif // RRWC_HPP
