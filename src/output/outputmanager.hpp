#ifndef OUTPUTMANAGER_HPP
#define OUTPUTMANAGER_HPP

#include <QVector>
#include <QString>

#include "gui/outputtab.hpp"
#include "output/engine/enginemanager.hpp"
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

    void startOutput(int output, const QString &inputPath, const QString &sort);
    void clean();

    QVector<Output *> outputs() const {
        return m_outputs;
    }

  public slots:
    void onProgressChanged(int output, int progress);
    void onDone();
    void onWriteLog(QString log, QString str);

  signals:
    void progressChanged(int progress);
    void done();
    void writeLog(QString log, QString str);

  private:
    void fillEntryList(QDir dir, const QString &sort);

    QStringList m_entryList;
    QVector<Output *> m_outputs;
    QVector<int> m_outputProgress;
    QVector<EngineManager *> m_engines;
    int m_outputsRemaining;
};

#endif // OUTPUTMANAGER_HPP
