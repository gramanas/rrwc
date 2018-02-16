#ifndef OUTPUTMANAGER_HPP
#define OUTPUTMANAGER_HPP

#include <QVector>
#include <QString>

#include "gui/outputtab.hpp"
#include "engine/enginemanager.hpp"
#include "output/entrylist.hpp"
#include "output.hpp"

class OutputManager : public QObject {
    Q_OBJECT
  public:
    OutputManager();
    ~OutputManager();
    void generateOutputsFromTabs(QVector<OutputTab *> outputTabs);
    void saveProfile(const QString &filename);
    bool loadProfile(const QString &filename);
    void print() const;

    void startOutput(int output);
    void clean();
    void fillEntryList(const QString &inputPath, const QString &sort);

    QVector<Output *> outputs() const {
        return m_outputs;
    }

  public slots:
    void onProgressChanged(int output, int progress);
    void onProgressChanged(int progress);
    void onStatusChanged(QString status);
    void slotEntryListFull();
    void onDone();
    void onWriteLog(QString log, QString str);

  signals:
    void progressChanged(int progress);
    void statusChanged(QString status);
    void entryListFull();
    void done();
    void writeLog(QString log, QString str);

  private:
    EntryList m_entryList;
    QVector<Output *> m_outputs;
    QVector<int> m_outputProgress;
    QVector<EngineManager *> m_engines;
    int m_outputsRemaining;
};

#endif // OUTPUTMANAGER_HPP
