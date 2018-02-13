#ifndef RRWC_HPP
#define RRWC_HPP

#include <QString>
#include <QTime>
#include <QApplication>

#include "output/outputmanager.hpp"
#include "error/errormanager.hpp"


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
    void onStatusChanged(QString status);
    void slotEntryListFull();
    void onDone();
    void onWriteLog(QString log, QString str);

  signals:
    void progressChanged(int progress);
    void statusChanged(QString status);
    void started();
    void writeLog(QString log, QString str);
    void done(int);

  private:
    void clearLogs();
    OutputManager m_outputManager;
    QTime m_totalTime;
};

#endif // RRWC_HPP
