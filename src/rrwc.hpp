#ifndef RRWC_HPP
#define RRWC_HPP

#include <QString>
#include <QApplication>

#include "outputmanager.hpp"

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

    void go(const QString &inputPath, const QString &sort);

  public slots:
    void onProgressChanged(int progress);
    void onDone();
    void onWriteLog(QString log, QString str);

  signals:
    void progressChanged(int progress);
    void started();
    void error(QStringList errors);
    void writeLog(QString log, QString str);
    void done(int);

  private:
    OutputManager m_outputManager;
};

#endif // RRWC_HPP
